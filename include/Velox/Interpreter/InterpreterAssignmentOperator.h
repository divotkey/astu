#pragma once

// Local includes
#include "InterpreterExpression.h"
#include "Operators.h"

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



