#include "InterpreterSimpleName.h"
#include "InterpreterException.h"
#include "ScriptContext.h"
#include "ItemStateUndefined.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterSimpleName::Evaluate(ScriptContext &sc) {
        auto result = sc.FindItem(name);
        if (!result) {
            if (!IsLocation()) {
                throw InterpreterException("Unknown identifier '" + name + "'");
            }
            result = make_shared<Item>(make_unique<ItemStateUndefined>());
            sc.AddItem(name, result);
        }

        return result;
    }

}
