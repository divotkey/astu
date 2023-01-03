/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterExpressionUnaryMinus.h"
#include "Velox/Interpreter/Item.h"

// C++ Standard Library
#include <cassert>

using namespace std;

namespace velox {

    InterpreterExpressionUnaryMinus::InterpreterExpressionUnaryMinus(unsigned int lineNumber,
                                                                     shared_ptr<InterpreterExpression> expr)
            : InterpreterExpression(lineNumber, false)
    {
        SetExpression(expr);
    }

    void InterpreterExpressionUnaryMinus::SetExpression(shared_ptr<InterpreterExpression> exp) {
        expression = exp;
        if (expression)
            expression->SetLocation(false);
    }

    shared_ptr<Item> velox::InterpreterExpressionUnaryMinus::Evaluate(velox::ScriptContext &sc) {
        assert(expression);
        auto result = expression->Evaluate(sc);
        return result->ExecuteUnaryMinus();
    }

}
