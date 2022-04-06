#include "InterpreterLogicalOperation.h"
#include "Item.h"
#include "ItemStateBool.h"

using namespace std;

namespace velox {

    InterpreterLogicalOperation::InterpreterLogicalOperation(LogicalOperator op)
        : InterpreterExpression(false), logOp(op)
    {
        // Intentionally left empty.
    }

    void InterpreterLogicalOperation::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        leftHandSide = lValue;
    }

    void InterpreterLogicalOperation::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rightHandSide = rValue;
    }

    shared_ptr<Item> velox::InterpreterLogicalOperation::Evaluate(velox::ScriptContext &sc) {
        auto lhs = leftHandSide->Evaluate(sc);
        if (logOp == LogicalOperator::OR) {
            if (!lhs->GetBooleanValue()) {
                return Item::Create(make_unique<ItemStateBool>(rightHandSide->Evaluate(sc)->GetBooleanValue()));
            } else {
                return Item::Create(make_unique<ItemStateBool>(false));
            }
        } else {
            // Logical and.
            if (lhs->GetBooleanValue()) {
                return Item::Create(make_unique<ItemStateBool>(rightHandSide->Evaluate(sc)->GetBooleanValue()));
            } else {
                return Item::Create(make_unique<ItemStateBool>(false));
            }
        }
    }

}
