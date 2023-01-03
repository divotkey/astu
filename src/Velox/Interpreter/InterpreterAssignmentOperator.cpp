/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterAssignmentOperator.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    InterpreterAssignmentOperator::InterpreterAssignmentOperator(ArithmeticOperator op, unsigned int lineNumber)
        : InterpreterExpression(lineNumber, false), arithOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterAssignmentOperator::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
    }

    void InterpreterAssignmentOperator::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
    }

    shared_ptr<Item> velox::InterpreterAssignmentOperator::Evaluate(velox::ScriptContext &sc) {
        auto lhsItem = leftHandSide->Evaluate(sc);
        auto rhsItem = rightHandSide->Evaluate(sc);

        auto resultItem = lhsItem->ExecuteArithmeticOperator(sc, arithOp, rhsItem, GetLineNumber());
        lhsItem->Assign(resultItem);

        return lhsItem;
    }


}
