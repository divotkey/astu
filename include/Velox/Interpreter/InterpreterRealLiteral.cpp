#include "InterpreterRealLiteral.h"
#include "Item.h"
#include "ItemStateReal.h"

using namespace std;

namespace velox {

    InterpreterRealLiteral::InterpreterRealLiteral(double value) : InterpreterExpression(false), value(value) {
        // Intentionally left empty.
    }

    std::shared_ptr<Item> velox::InterpreterRealLiteral::Evaluate(velox::ScriptContext &sc) {
        return make_shared<Item>(make_unique<ItemStateReal>(value));
    }

}
