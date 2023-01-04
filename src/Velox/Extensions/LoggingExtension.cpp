/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Extensions/LoggingExtension.h"
#include "Velox/Interpreter/InterpreterFunction.h"
#include "Velox/Service/VeloxConstants.h"
#include "Velox/Interpreter/Item.h"
#include "Service/LoggingService.h"

using namespace velox;
using namespace std;

namespace astu {

    class LogFunction : public InterpreterFunction {
    public:

        LogFunction(astu::LogLevel level) : level(level)
        {
            AddFormalParameter("msg");
        }

    protected:
        /** The log level of this logging function. */
        astu::LogLevel level;

        // Inherited via LogicFunction
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
            auto &item = sc.GetItem("msg", lineNumber);
            ASTU_SERVICE(LoggingService).Log(level, VeloxConstants::LOGGING_TAG, item.GetStringValue(sc));
            return Item::Create(make_unique<ItemStateUndefined>());
        }
    };

    void LoggingExtension::InjectExtension(Interpreter &interpreter) const
    {
        interpreter.AddGlobalFunction("LogVerbose", make_shared<LogFunction>(LogLevel::Verbose));
        interpreter.AddGlobalFunction("LogDebug", make_shared<LogFunction>(LogLevel::Debug));
        interpreter.AddGlobalFunction("LogInfo", make_shared<LogFunction>(LogLevel::Info));
        interpreter.AddGlobalFunction("LogConfig", make_shared<LogFunction>(LogLevel::Config));
        interpreter.AddGlobalFunction("LogWarning", make_shared<LogFunction>(LogLevel::Warn));
        interpreter.AddGlobalFunction("LogError", make_shared<LogFunction>(LogLevel::Err));
        interpreter.AddGlobalFunction("LogFatal", make_shared<LogFunction>(LogLevel::Fatal));
    }

    LoggingExtension::LoggingExtension()
        : VeloxExtension("Logging Extension")
    {
        // Intentionally left empty.
    }

} // end of namespace