// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"
#include "Velox/Interpreter/Operators.h"

namespace velox {

    class InterpreterAssignmentOperator : public InterpreterExpression {
    public:

        InterpreterAssignmentOperator(ArithmeticOperator op);
        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue);
        void SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue);

        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        ArithmeticOperator arithOp;
        std::shared_ptr<InterpreterExpression> leftHandSide;
        std::shared_ptr<InterpreterExpression> rightHandSide;
    };

}



