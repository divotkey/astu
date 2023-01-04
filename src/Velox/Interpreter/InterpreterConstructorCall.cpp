/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterConstructorCall.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterConstructorCall::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        parameters.AddParameter(param);
    }

    void InterpreterConstructorCall::Execute(ScriptContext &sc, Item &function) {
        // No function scope here, will be added during the parameter evaluation.
        function.CallAsFunction(sc, parameters, lineNumber);
    }
}
