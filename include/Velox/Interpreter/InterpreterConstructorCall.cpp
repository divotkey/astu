// Local includes
#include "InterpreterConstructorCall.h"
#include "Item.h"

namespace velox {

    void InterpreterConstructorCall::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        parameters.AddParameter(param);
    }

    void InterpreterConstructorCall::Execute(ScriptContext &sc, Item &function) {
        sc.PushScope();
        function.CallAsFunction(sc, parameters, lineNumber);
        sc.PopScope();
    }
}
