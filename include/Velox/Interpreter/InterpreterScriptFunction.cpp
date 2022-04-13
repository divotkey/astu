#include "InterpreterScriptFunction.h"
#include "Item.h"
#include "ItemStateUndefined.h"

using namespace std;

namespace velox {

    std::shared_ptr<Item> InterpreterScriptFunction::DoEvaluate(ScriptContext &sc) {
        sc.PushReturnValue();
        statement->Execute(sc);
        sc.ClearFlag(ScriptContext::RETURN_EXECUTED_FLAG);

        auto result = sc.PopReturnValue();
        if (!result)
            result = Item::Create(make_unique<ItemStateUndefined>());

        return result;
    }

    void InterpreterScriptFunction::SetStatement(std::shared_ptr<InterpreterStatement> inStatement) {
        statement = inStatement;
    }

}

