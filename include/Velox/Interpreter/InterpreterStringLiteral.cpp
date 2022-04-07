#include "InterpreterStringLiteral.h"
#include "Item.h"
#include "ItemStateString.h"

using namespace std;

namespace velox {

    InterpreterStringLiteral::InterpreterStringLiteral(const std::string &value)
        : InterpreterExpression(false), value(value)
    {
        // Intentionally left empty.
    }

    std::shared_ptr<Item> velox::InterpreterStringLiteral::Evaluate(velox::ScriptContext &sc) {
        return Item::Create(std::make_unique<ItemStateString>(value));
    }

}
