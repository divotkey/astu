/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterLogicalOperation.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateBool.h"

using namespace std;

namespace velox {

    InterpreterLogicalOperation::InterpreterLogicalOperation(LogicalOperator op, unsigned int lineNumber)
        : InterpreterExpression(lineNumber, false), logOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterLogicalOperation::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
    }

    void InterpreterLogicalOperation::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
    }

    shared_ptr<Item> velox::InterpreterLogicalOperation::Evaluate(velox::ScriptContext &sc) {
        auto lhs = leftHandSide->Evaluate(sc);
        if (logOp == LogicalOperator::OR) {
            if (!lhs->GetBooleanValue()) {
                return Item::Create(make_unique<ItemStateBool>(
                        rightHandSide->Evaluate(sc)->GetBooleanValue(rightHandSide->GetLineNumber())));
            } else {
                return Item::Create(make_unique<ItemStateBool>(true));
            }
        } else {
            // Logical and.
            if (lhs->GetBooleanValue()) {
                return Item::Create(make_unique<ItemStateBool>(
                        rightHandSide->Evaluate(sc)->GetBooleanValue(rightHandSide->GetLineNumber())));
            } else {
                return Item::Create(make_unique<ItemStateBool>(false));
            }
        }
    }

}
