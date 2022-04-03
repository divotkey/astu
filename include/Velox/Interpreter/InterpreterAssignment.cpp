#include "InterpreterAssignment.h"
#include "ScriptContext.h"
#include "Item.h"
#include "ItemStateUndefined.h"

#include <cassert>

using namespace std;

namespace velox {

    void InterpreterAssignment::SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue) {
        assert(lValue->IsLocation());
        lhs = lValue;
    }

    void InterpreterAssignment::SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue) {
        rhs = rValue;
    }

    shared_ptr<Item> InterpreterAssignment::Evaluate(ScriptContext &sc) {
        auto rightItem = rhs->Evaluate(sc);
        auto leftItem = lhs->Evaluate(sc);
        leftItem->Assign(*rightItem);
        return leftItem;
    }

}