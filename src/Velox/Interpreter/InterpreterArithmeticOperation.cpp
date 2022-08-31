// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/InterpreterArithmeticOperation.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    InterpreterArithmeticOperation::InterpreterArithmeticOperation(ArithmeticOperator op, unsigned int lineNumber)
        : InterpreterExpression(lineNumber, false), arithOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterArithmeticOperation::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
        leftHandSide->SetLocation(false);
    }

    void InterpreterArithmeticOperation::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
        rightHandSide->SetLocation(false);
    }

    shared_ptr<Item> velox::InterpreterArithmeticOperation::Evaluate(velox::ScriptContext &sc) {
        auto lhs = leftHandSide->Evaluate(sc);
        auto rhs = rightHandSide->Evaluate(sc);

        return lhs->ExecuteArithmeticOperator(sc, arithOp, rhs, GetLineNumber());
    }


}
