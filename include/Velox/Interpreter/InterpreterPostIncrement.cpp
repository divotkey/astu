#include "InterpreterPostIncrement.h"
#include "Item.h"
#include "ItemStateInteger.h"

using namespace std;

namespace velox {

    std::shared_ptr<Item> InterpreterPostIncrement::Evaluate(ScriptContext &sc) {
        auto itemValue = value->Evaluate(sc);

        auto prevValue = itemValue->Copy();

        auto result = itemValue->ExecuteArithmeticOperator(
                decrement ? ArithmeticOperator::SUB : ArithmeticOperator::ADD,
                *Item::Create(make_unique<ItemStateInteger>(1)));

        itemValue->Assign(result);

        return prevValue;
    }

    void InterpreterPostIncrement::SetValue(std::shared_ptr<InterpreterExpression> lValue) {
        value = lValue;
    }
}
