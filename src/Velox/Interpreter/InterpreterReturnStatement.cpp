/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterReturnStatement.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateUndefined.h"

using namespace std;

namespace velox {

    void InterpreterReturnStatement::Execute(ScriptContext &sc) {
        if (expression) {
            sc.SetCurrentReturnValue(expression->Evaluate(sc));
        } else {
            sc.SetCurrentReturnValue(Item::Create(std::make_unique<ItemStateUndefined>()));
        }
        sc.SetFlag(ScriptContext::RETURN_EXECUTED_FLAG);
    }

    void InterpreterReturnStatement::SetReturnExpression(std::shared_ptr<InterpreterExpression> inExpression) {
        expression = inExpression;
    }

}