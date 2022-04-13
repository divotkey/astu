#include "InterpreterRelationalOperation.h"
#include "Item.h"

using namespace std;

namespace velox {

    InterpreterRelationalOperation::InterpreterRelationalOperation(RelationalOperator op)
        : InterpreterExpression(false), relOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterRelationalOperation::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
    }

    void InterpreterRelationalOperation::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
    }

    shared_ptr<Item> velox::InterpreterRelationalOperation::Evaluate(velox::ScriptContext &sc) {
        auto lhs = leftHandSide->Evaluate(sc);
        auto rhs = rightHandSide->Evaluate(sc);

        return lhs->ExecuteRelationalOperator(sc, relOp, *rhs);
    }


}
