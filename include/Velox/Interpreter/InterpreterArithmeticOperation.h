#pragma once

// Local includes
#include "InterpreterExpression.h"
#include "Operators.h"

namespace velox {

    class InterpreterArithmeticOperation : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param op            the arithmetic operator
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterArithmeticOperation(ArithmeticOperator op, unsigned int lineNumber);

        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue);
        void SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        ArithmeticOperator arithOp;
        std::shared_ptr<InterpreterExpression> leftHandSide;
        std::shared_ptr<InterpreterExpression> rightHandSide;
    };

}



