#include "InterpreterFunctionDefinition.h"
#include "InterpreterError.h"
#include "Item.h"
#include "ItemStateFunction.h"

using namespace std;

namespace velox {

    void InterpreterFunctionDefinition::Execute(ScriptContext &sc) {
        if (sc.GetCurrentScope().HasItem(functionName)) {
            throw InterpreterError("Ambiguous function name '" + functionName + "'");
        }

        sc.GetCurrentScope().AddItem(functionName, make_shared<Item>(make_unique<ItemStateFunction>(function)));
    }

    void InterpreterFunctionDefinition::SetFunction(std::shared_ptr<InterpreterScriptFunction> inFunction) {
        function = inFunction;
    }

    void InterpreterFunctionDefinition::SetFunctionName(const std::string &name) {
        functionName = name;
    }

}