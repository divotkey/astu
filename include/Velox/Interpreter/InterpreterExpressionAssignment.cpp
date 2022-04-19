#include "InterpreterExpressionAssignment.h"
#include "ScriptContext.h"
#include "Item.h"
#include "ItemStateUndefined.h"

#include <cassert>

using namespace std;

namespace velox {

    void InterpreterExpressionAssignment::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        assert(lValue->IsLocation());
        lhs = lValue;
    }

    void InterpreterExpressionAssignment::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rhs = rValue;
        rhs->SetLocation(false);
    }

    shared_ptr<Item> InterpreterExpressionAssignment::Evaluate(ScriptContext &sc) {
        auto rightItem = rhs->Evaluate(sc);
        auto leftItem = lhs->Evaluate(sc);

        leftItem->Assign(rightItem);
        return leftItem;
    }

}