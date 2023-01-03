/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterPostIncrement.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateInteger.h"

using namespace std;

namespace velox {

    std::shared_ptr<Item> InterpreterPostIncrement::Evaluate(ScriptContext &sc) {
        auto itemValue = value->Evaluate(sc);

        auto prevValue = itemValue->Copy();

        auto result = itemValue->ExecuteArithmeticOperator(sc,
                                                           decrement ? ArithmeticOperator::SUB
                                                                     : ArithmeticOperator::ADD,
                                                           Item::Create(make_unique<ItemStateInteger>(1)), GetLineNumber());

        itemValue->Assign(result);

        return prevValue;
    }

    void InterpreterPostIncrement::SetValue(std::shared_ptr<InterpreterExpression> lValue) {
        value = lValue;
    }
}
