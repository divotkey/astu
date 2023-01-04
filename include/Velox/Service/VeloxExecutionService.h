/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/SubstateService.h"
#include "Service/ThreadService.h"
#include "Service/LoggingService.h"
#include "Util/Timer.h"
#include "Velox/Parser/FastSource.h"
#include "Velox/Parser/Parser.h"
#include "Velox/Interpreter/Interpreter.h"
#include "Velox/Interpreter/InterpreterScript.h"

// C++ Standard Library includes
#include <memory>
#include <queue>
#include <string>

namespace astu {

    // Forward declaration
    class VeloxExecutionService;

    /////////////////////////////////////////////////
    /////// ExecutionServiceSubstate
    /////////////////////////////////////////////////

    /**
     * Base class for substates of VeloxExecutionService.
     * This class cannot be an inner class because forward declaration of inner classes is not possible
     * in C++.
     */
    class ExecutionServiceSubstate : public Substate<VeloxExecutionService> {
    public:
        ExecutionServiceSubstate(VeloxExecutionService &context) : Substate(context) {}
        virtual void ExecuteScript(const std::string& filename) = 0;
        virtual bool OnThreadSuccess(int threadId) { return false; }
        virtual bool OnThreadError(int threadId, const std::string &message) { return false; }
    };

    /////////////////////////////////////////////////
    /////// VeloxExecutionService
    /////////////////////////////////////////////////

    class VeloxExecutionService
            : virtual public SubstateService<ExecutionServiceSubstate>
            , public ThreadClient
            , public LoggingClient
    {
    public:

        /** Constructor. */
        VeloxExecutionService();

        /**
         * Loads, parses and executes the specified script.
         *
         * @param filename  the filename including the path to the script to be executed
         */
        void ExecuteScript(const std::string &filename);

    protected:

        // Inherited via Service
        void OnStartup() override;
        void OnShutdown() override;

        // Inherited via ThreadClient
        bool OnThreadSuccess(int threadId) override;
        bool OnThreadError(int threadId, const std::string &message) override;

    private:

        /** Substate when nothing needs to be done. */
        class Idle : public ExecutionServiceSubstate {
        public:
            Idle(VeloxExecutionService &context) : ExecutionServiceSubstate(context) {}

            // Inherited via ExecutionServiceSubstate
            void ExecuteScript(const std::string& filename) override;
        };

        /** Base class for states that execute Velox related tasks (parsing, interpreting, etc). */
        class VeloxState : public ExecutionServiceSubstate {
        public:
            VeloxState(VeloxExecutionService &context);

            // Inherited via ExecutionServiceSubstate
            void OnEnter() override;

        protected:
            /** The thread identifier of the parsing thread. */
            int curThreadId;

            /** Used to measure parsing time. */
            Timer timer;

            /** The error cause, in case something went wrong. */
            std::string errorText;

            /** The line number within the script, where the error has occurred. */
            unsigned int lineNumber;

            /** Indicates that an error has occurred while executing the Velox task. */
            bool error;
        };

        /** Parses the next script in the execution queue. */
        class Parsing : public VeloxState {
        public:
            Parsing(VeloxExecutionService &context) : VeloxState(context) {}

            // Inherited via ExecutionServiceSubstate
            void ExecuteScript(const std::string& filename) override;
            void OnEnter() override;
            bool OnThreadSuccess(int threadId) override;
            bool OnThreadError(int threadId, const std::string &message) override;

        private:
            /** The file source used for parsing. */
            velox::FastFileSource fileSource;

            /** The Velox parser used to parse script. */
            velox::Parser parser;
        };

        class Executing : public VeloxState {
        public:

            Executing(VeloxExecutionService &context);

            // Inherited via ExecutionServiceSubstate
            void ExecuteScript(const std::string& filename) override;
            void OnEnter() override;
            bool OnThreadSuccess(int threadId) override;
            bool OnThreadError(int threadId, const std::string &message) override;

        private:
            /** The thread identifier of the executing thread. */
            int threadId;

            /** Used to measure execution time. */
            Timer timer;

            /** The velox interpreter used to execute scripts. */
            std::unique_ptr<velox::Interpreter> interpreter;

            /** Stores the number of global scopes before interpreter is used. */
            size_t numGlobalScopes;
        };

    private:
        /** Queue with file names of scripts to be executed. */
        std::queue<std::string> executionQueue;

        /** The parsed script. */
        std::shared_ptr<velox::InterpreterScript> script;


        /**
         * Removes all pending scripts to be executed.
         */
        void ClearExecutionQueue();

        /**
         * Creates and initializes a new Velox interpreter.
         *
         * @return  the newly created Velox interpreter
         */
        std::unique_ptr<velox::Interpreter> CreateInterpreter();
    };


} // end of namespace

