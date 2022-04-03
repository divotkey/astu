#include "InterpreterIntegerLiteral.h"
#include "Item.h"
#include "ItemStateInteger.h"

using namespace std;

namespace velox {

    InterpreterIntegerLiteral::InterpreterIntegerLiteral(int value) : InterpreterExpression(false), value(value) {
        // Intentionally left empty.
    }

    std::shared_ptr<Item> velox::InterpreterIntegerLiteral::Evaluate(velox::ScriptContext &sc) {
        return make_shared<Item>(make_unique<ItemStateInteger>(value));
    }

}
