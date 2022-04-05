#include "InterpreterScriptFunction.h"

namespace velox {

    std::shared_ptr<Item> InterpreterScriptFunction::DoEvaluate(ScriptContext &sc) {
        sc.PushReturnValue();
        statement->Execute(sc);
        sc.ClearFlag(ScriptContext::RETURN_EXECUTED_FLAG);
        return sc.PopReturnValue();
    }

    void InterpreterScriptFunction::SetStatement(std::shared_ptr<InterpreterStatement> inStatement) {
        statement = inStatement;
    }

}

