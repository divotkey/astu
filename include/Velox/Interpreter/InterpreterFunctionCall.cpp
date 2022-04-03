#include "InterpreterFunctionCall.h"

#include "Item.h"

namespace velox {

    void InterpreterFunctionCall::SetFunction(std::shared_ptr<InterpreterExpression> inFunction) {
        function = inFunction;
    }

    void InterpreterFunctionCall::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        parameters.AddParameter(param);
    }

    std::shared_ptr<Item> InterpreterFunctionCall::Evaluate(ScriptContext &sc) {
        auto funcItem = function->Evaluate(sc);
        return funcItem->CallAsFunction(sc, parameters);
    }

}