#include "InterpreterMemberAccess.h"
#include "InterpreterError.h"
#include "ItemStateUndefined.h"
#include "Item.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterMemberAccess::Evaluate(ScriptContext &sc) {
        auto leftItem = leftHandSide->Evaluate(sc);

        // Make sure item stays alive after evaluation of this member access; required for temporary objects
        sc.AddItem(leftItem);

        auto result = leftItem->FindItem(name);

        if (!result) {
            if (!IsLocation()) {
                throw InterpreterError("Unknown field '" + name + "'", GetLineNumber());
            }
            result = Item::Create(make_unique<ItemStateUndefined>());
            leftItem->AddItem(name, result);
        }
        return result;
    }

    void InterpreterMemberAccess::SetLeftHandSide(shared_ptr<InterpreterExpression> lhs) {
        leftHandSide = lhs;
    }

    void InterpreterMemberAccess::SetRightHandSide(const string &inName) {
        name = inName;
    }

}