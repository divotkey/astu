// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

#include "Velox/Interpreter/InterpreterExpression.h"
#include "Velox/Interpreter/Operators.h"

namespace velox {

    class InterpreterRelationalOperation : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param op            the relational operator
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterRelationalOperation(RelationalOperator op, unsigned int lineNumber);

        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue);
        void SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        RelationalOperator relOp;
        std::shared_ptr<InterpreterExpression> leftHandSide;
        std::shared_ptr<InterpreterExpression> rightHandSide;
    };

}



