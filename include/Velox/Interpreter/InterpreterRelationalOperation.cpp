#include "InterpreterRelationalOperation.h"
#include "Item.h"

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

        return lhs->ExecuteRelationalOperator(sc, relOp, *rhs, GetLineNumber());
    }


}
