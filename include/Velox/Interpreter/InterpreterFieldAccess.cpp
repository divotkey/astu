#include "InterpreterFieldAccess.h"
#include "InterpreterException.h"
#include "ItemStateUndefined.h"
#include "Item.h"

using namespace std;

namespace velox {

    InterpreterFieldAccess::InterpreterFieldAccess(bool location) : InterpreterExpression(location) {
        // Intentionally left empty.
    }

    shared_ptr<Item> InterpreterFieldAccess::Evaluate(ScriptContext &sc) {
        auto leftItem = leftHandSide->Evaluate(sc);
        auto result = leftItem->FindItem(name);
        if (!result) {
            if (!IsLocation()) {
                throw InterpreterException("Unknown field '" + name + "'");
            }
            result = make_shared<Item>(make_unique<ItemStateUndefined>());
            leftItem->AddItem(name, result);
        }

        return result;
    }

    void InterpreterFieldAccess::SetLeftHandSide(shared_ptr<InterpreterExpression> lhs) {
        leftHandSide = lhs;
    }

    void InterpreterFieldAccess::SetRightHandSide(const string &inName) {
        name = inName;
    }

}