#include "InterpreterAssignmentOperator.h"
#include "Item.h"

using namespace std;

namespace velox {

    InterpreterAssignmentOperator::InterpreterAssignmentOperator(ArithmeticOperator op)
        : InterpreterExpression(false), arithOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterAssignmentOperator::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
    }

    void InterpreterAssignmentOperator::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
    }

    shared_ptr<Item> velox::InterpreterAssignmentOperator::Evaluate(velox::ScriptContext &sc) {
        auto lhsItem = leftHandSide->Evaluate(sc);
        auto rhsItem = rightHandSide->Evaluate(sc);

        lhsItem->Assign(*lhsItem->ExecuteArithmeticOperator(arithOp, *rhsItem));
        return lhsItem;
    }


}
