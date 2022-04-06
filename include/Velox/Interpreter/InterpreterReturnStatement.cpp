#include "InterpreterReturnStatement.h"
#include "Item.h"
#include "ItemStateUndefined.h"

using namespace std;

namespace velox {

    void InterpreterReturnStatement::Execute(ScriptContext &sc) {
        if (expression) {
            sc.SetCurrentReturnValue(expression->Evaluate(sc));
        } else {
            sc.SetCurrentReturnValue(Item::Create(std::make_unique<ItemStateUndefined>()));
        }
        sc.SetFlag(ScriptContext::RETURN_EXECUTED_FLAG);
    }

    void InterpreterReturnStatement::SetReturnExpression(std::shared_ptr<InterpreterExpression> inExpression) {
        expression = inExpression;
    }

}