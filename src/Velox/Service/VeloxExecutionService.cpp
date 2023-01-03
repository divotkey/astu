/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Service/VeloxExecutionService.h"
#include "Velox/Service/VeloxExtensionService.h"
#include "Velox/Service/VeloxConstants.h"
#include "Util/StringUtils.h"

// C++ Standard Library includes
#include <cassert>

using namespace velox;
using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// VeloxExecutionService
    /////////////////////////////////////////////////

    VeloxExecutionService::VeloxExecutionService() : Service("Velox Execution Service")
    {
        AddSubstate("IDLE", make_unique<Idle>(*this));
        AddSubstate("PARSING", make_unique<Parsing>(*this));
        AddSubstate("EXECUTING", make_unique<Executing>(*this));
        SetStartState("IDLE");
    }

    void VeloxExecutionService::OnStartup()
    {
        // Intentionally left empty.
    }

    void VeloxExecutionService::OnShutdown()
    {
        ClearExecutionQueue();
    }

    bool VeloxExecutionService::OnThreadSuccess(int threadId)
    {
        return GetSubstate(GetCurrentSubstate()).OnThreadSuccess(threadId);
    }

    bool VeloxExecutionService::OnThreadError(int threadId, const string &message)
    {
        return GetSubstate(GetCurrentSubstate()).OnThreadError(threadId, message);
    }

    std::unique_ptr<velox::Interpreter> VeloxExecutionService::CreateInterpreter()
    {
        auto interpreter = make_unique<Interpreter>();

        // Global scope with standard functions and constants.
        interpreter->PushGlobalScope();
        interpreter->AddStandardGlobals();

        // Global scope for extensions
        interpreter->PushGlobalScope();
        ASTU_SERVICE(VeloxExtensionService).InjectExtensions(*interpreter);

        return interpreter;
    }

    void VeloxExecutionService::ClearExecutionQueue()
    {
        while (!executionQueue.empty()) {
            executionQueue.pop();
        }
    }

    void VeloxExecutionService::ExecuteScript(const string &filename)
    {
        GetSubstate(GetCurrentSubstate()).ExecuteScript(filename);
    }


    /////////////////////////////////////////////////
    /////// Idle
    /////////////////////////////////////////////////

    void VeloxExecutionService::Idle::ExecuteScript(const std::string &filename)
    {
        context.executionQueue.push(filename);
        context.SwitchSubstate("PARSING");
    }

    /////////////////////////////////////////////////
    /////// VeloxState
    /////////////////////////////////////////////////

    VeloxExecutionService::VeloxState::VeloxState(VeloxExecutionService &context)
            : ExecutionServiceSubstate(context)
            , curThreadId(ThreadService::INVALID_THREAD_ID)
            , error(false)
            , lineNumber(0)
    {
        // Intentionally left empty.
    }

    void VeloxExecutionService::VeloxState::OnEnter()
    {
        curThreadId = ThreadService::INVALID_THREAD_ID;
        errorText.clear();
        lineNumber = 0;
        error = false;
    }

    /////////////////////////////////////////////////
    /////// Parsing
    /////////////////////////////////////////////////

    void VeloxExecutionService::Parsing::OnEnter()
    {
        assert(!context.executionQueue.empty());
        VeloxState::OnEnter();

        assert(!context.executionQueue.empty());
        fileSource.Reset(context.executionQueue.front());
        context.LogDebug(VeloxConstants::LOGGING_TAG, "Parsing Velox script '" + fileSource.GetFilepath() + "'");

        timer.Start();
        curThreadId = context.StartThread([this](int threadId) {
            try {
                context.script = parser.Parse(fileSource);
            } catch (const ParserError &e) {
                error = true;
                errorText = e.what();
                lineNumber = e.GetLineNumber();
            }
        });
    }

    void VeloxExecutionService::Parsing::ExecuteScript(const string &filename)
    {
        context.executionQueue.push(filename);
        context.LogInfo(VeloxConstants::LOGGING_TAG, "Scheduled script '"
            + filename + "' + for execution (parsing other script at the moment)");
    }

    bool VeloxExecutionService::Parsing::OnThreadSuccess(int threadId)
    {
        if (threadId != curThreadId)
            return false;

        timer.Stop();

        if (error) {
            context.LogError(
                    VeloxConstants::LOGGING_TAG,
                    "Error parsing Velox script '"
                    + fileSource.GetFilepath() + "': " + errorText  + " at line " + to_string(lineNumber));

            assert(!context.executionQueue.empty());
            assert(context.executionQueue.front() == fileSource.GetFilepath());
            context.executionQueue.pop();
            context.SwitchSubstate("IDLE");
        } else {
            context.LogDebug(
                    VeloxConstants::LOGGING_TAG,
                    "Successfully parsed Velox script '"
                    + fileSource.GetFilepath() + "' in " + to_string(timer.GetMilliseconds()) + " ms");

            context.SwitchSubstate("EXECUTING");
        }

        return true;
    }

    bool VeloxExecutionService::Parsing::OnThreadError(int threadId, const string &message)
    {
        if (threadId != curThreadId)
            return false;

        timer.Stop();
        context.LogError(VeloxConstants::LOGGING_TAG, "Error while executing parser thread: " + message);
        context.SwitchSubstate("IDLE");

        return true;
    }


    /////////////////////////////////////////////////
    /////// Executing
    /////////////////////////////////////////////////

    VeloxExecutionService::Executing::Executing(VeloxExecutionService &context)
        : VeloxState(context)
    {
        // Intentionally left empty.
    }

    void VeloxExecutionService::Executing::OnEnter()
    {
        assert(context.script != nullptr);
        assert(!context.executionQueue.empty());

        VeloxState::OnEnter();

        assert(!context.executionQueue.empty());
        context.LogDebug(VeloxConstants::LOGGING_TAG, "Executing Velox script '" + context.executionQueue.front() + "'");

        if (!interpreter) {
            interpreter = context.CreateInterpreter();
        }

        numGlobalScopes = interpreter->NumGlobalScopes();
        interpreter->PushGlobalScope();

        timer.Start();
        curThreadId = context.StartThread([this](int threadId) {
            try {
                interpreter->Execute(*context.script);
            } catch (const InterpreterError &e) {
                error = true;
                errorText = e.what();
                lineNumber = e.GetLineNumber();
            }
        });

    }

    void VeloxExecutionService::Executing::ExecuteScript(const std::string &filename)
    {
        context.executionQueue.push(filename);
        context.LogInfo(VeloxConstants::LOGGING_TAG, "Scheduled script '" + filename + "' + for execution (executing other script at the moment)");
    }

    bool VeloxExecutionService::Executing::OnThreadSuccess(int threadId)
    {
        if (threadId != curThreadId)
            return false;

        assert(!context.executionQueue.empty());
        timer.Stop();

        if (error) {
            context.LogError(
                    VeloxConstants::LOGGING_TAG,
                    "Error interpreting Velox script '"
                    + context.executionQueue.front() + +"': " + errorText + " at line " + to_string(lineNumber));
        } else {
            context.LogInfo(
                    VeloxConstants::LOGGING_TAG,
                    "Successfully executed Velox script '"
                    + context.executionQueue.front() + "' in " + to_string(timer.GetMilliseconds()) + " ms");
        }

        interpreter->PopGlobalScope();
        assert(numGlobalScopes == interpreter->NumGlobalScopes());
        context.executionQueue.pop();
        context.SwitchSubstate("IDLE");

        return true;
    }

    bool VeloxExecutionService::Executing::OnThreadError(int threadId, const string &message)
    {
        if (threadId != curThreadId)
            return false;

        timer.Stop();
        context.LogError(VeloxConstants::LOGGING_TAG, "Error while executing interpreter thread: " + message);

        interpreter->PopGlobalScope();
        assert(numGlobalScopes == interpreter->NumGlobalScopes());

        context.executionQueue.pop();
        context.SwitchSubstate("IDLE");
        return true;
    }

} // end of namespace