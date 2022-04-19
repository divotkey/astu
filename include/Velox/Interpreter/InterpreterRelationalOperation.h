#pragma once

#include "InterpreterExpression.h"
#include "Operators.h"

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



