#include "InterpreterArithmeticOperation.h"
#include "Item.h"

using namespace std;

namespace velox {

    InterpreterArithmeticOperation::InterpreterArithmeticOperation(ArithmeticOperator op, unsigned int lineNumber)
        : InterpreterExpression(lineNumber, false), arithOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterArithmeticOperation::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
    }

    void InterpreterArithmeticOperation::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
    }

    shared_ptr<Item> velox::InterpreterArithmeticOperation::Evaluate(velox::ScriptContext &sc) {
        auto lhs = leftHandSide->Evaluate(sc);
        auto rhs = rightHandSide->Evaluate(sc);

        return lhs->ExecuteArithmeticOperator(sc, arithOp, rhs, GetLineNumber());
    }


}
