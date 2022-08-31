// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/InterpreterRelationalOperation.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    InterpreterRelationalOperation::InterpreterRelationalOperation(RelationalOperator op, unsigned int lineNumber)
        : InterpreterExpression(lineNumber, false), relOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterRelationalOperation::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
        leftHandSide->SetLocation(false);
    }

    void InterpreterRelationalOperation::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
        rightHandSide->SetLocation(false);
    }

    shared_ptr<Item> velox::InterpreterRelationalOperation::Evaluate(velox::ScriptContext &sc) {
        auto lhs = leftHandSide->Evaluate(sc);
        auto rhs = rightHandSide->Evaluate(sc);

        return lhs->ExecuteRelationalOperator(sc, relOp, rhs, GetLineNumber());
    }


}
