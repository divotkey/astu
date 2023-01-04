/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterActualParameterList.h"

namespace velox {

    // Forward declaration.
    class ScriptContext;
    class Item;

    class InterpreterConstructorCall {
    public:

        InterpreterConstructorCall(unsigned int lineNumber) : lineNumber(lineNumber) {}

        void AddParameter(std::shared_ptr<InterpreterExpression> param);
        void Execute(ScriptContext &sc, Item& function);

        size_t NumParameters() const {
            return parameters.NumParameters();
        }

    private:
        /** The actual parameters of this function call. */
        InterpreterExpressionParameterList parameters;

        /** The line number of this constructor call. */
        unsigned int lineNumber;
    };

}


