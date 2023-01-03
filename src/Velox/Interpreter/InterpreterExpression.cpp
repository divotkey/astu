/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    void InterpreterExpression::Execute(ScriptContext &sc) {
        // Evaluate this expression and ignore the result.
        Evaluate(sc);
    }

}
