/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterExpressionNot.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    InterpreterExpressionNot::InterpreterExpressionNot(std::shared_ptr<InterpreterExpression> expr, unsigned int lineNumber)
        : InterpreterExpression(lineNumber, false), expression(expr)
    {
        // Intentionally left empty.
    }

    void InterpreterExpressionNot::SetExpression(std::shared_ptr<InterpreterExpression> inExpression) {
        expression = inExpression;
        expression->SetLocation(false);
    }

    shared_ptr<Item> velox::InterpreterExpressionNot::Evaluate(velox::ScriptContext &sc) {
        auto result = expression->Evaluate(sc);
        return Item::CreateBoolean(!result->GetBooleanValue(GetLineNumber()));
    }


} // end of namespace
