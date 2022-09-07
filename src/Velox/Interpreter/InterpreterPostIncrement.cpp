// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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