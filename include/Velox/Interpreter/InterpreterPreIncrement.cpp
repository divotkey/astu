#include "InterpreterPreIncrement.h"
#include "Item.h"
#include "ItemStateInteger.h"

using namespace std;

namespace velox {

    std::shared_ptr<Item> InterpreterPreIncrement::Evaluate(ScriptContext &sc) {
        auto itemValue = value->Evaluate(sc);

        auto result = itemValue->ExecuteArithmeticOperator(
                decrement ? ArithmeticOperator::SUB : ArithmeticOperator::ADD,
                *Item::Create(make_unique<ItemStateInteger>(1)));

        itemValue->Assign(result);
        return itemValue;
    }

    void InterpreterPreIncrement::SetValue(std::shared_ptr<InterpreterExpression> lValue) {
        value = lValue;
    }
}
