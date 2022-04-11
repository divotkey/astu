#include "InterpreterFunctionCall.h"
#include "InterpreterError.h"
#include "Item.h"

namespace velox {

    void InterpreterFunctionCall::SetFunction(std::shared_ptr<InterpreterExpression> inFunction) {
        function = inFunction;
    }

    void InterpreterFunctionCall::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        // Parameters should not be handled as l-values even if it's a variable and hence a location.
        param->SetLocation(false);
        parameters.AddParameter(param);
    }

    std::shared_ptr<Item> InterpreterFunctionCall::Evaluate(ScriptContext &sc) {
        auto funcItem= function->Evaluate(sc);
        sc.PushScope();
        auto result = funcItem->CallAsFunction(sc, parameters, GetLineNumber());
        sc.PopScope();
        return result;
    }

}