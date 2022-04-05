#pragma once

#include "InterpreterExpression.h"
#include "Operators.h"

namespace velox {

    class InterpreterRelationalOperation : public InterpreterExpression {
    public:

        InterpreterRelationalOperation(RelationalOperator op);
        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue);
        void SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue);

        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        RelationalOperator relOp;
        std::shared_ptr<InterpreterExpression> leftHandSide;
        std::shared_ptr<InterpreterExpression> rightHandSide;
    };

}



