// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
