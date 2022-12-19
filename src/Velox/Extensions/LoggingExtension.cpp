/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
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
        interpreter.AddFunction("LogVerbose", make_shared<LogFunction>(LogLevel::Verbose));
        interpreter.AddFunction("LogDebug", make_shared<LogFunction>(LogLevel::Debug));
        interpreter.AddFunction("LogInfo", make_shared<LogFunction>(LogLevel::Info));
        interpreter.AddFunction("LogConfig", make_shared<LogFunction>(LogLevel::Config));
        interpreter.AddFunction("LogWarning", make_shared<LogFunction>(LogLevel::Warn));
        interpreter.AddFunction("LogError", make_shared<LogFunction>(LogLevel::Err));
        interpreter.AddFunction("LogFatal", make_shared<LogFunction>(LogLevel::Fatal));
    }

    LoggingExtension::LoggingExtension()
        : VeloxExtension("Logging Extension")
    {
        // Intentionally left empty.
    }

} // end of namespace