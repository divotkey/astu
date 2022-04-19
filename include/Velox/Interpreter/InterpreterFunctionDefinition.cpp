#include "InterpreterFunctionDefinition.h"
#include "InterpreterError.h"
#include "Item.h"
#include "ItemStateFunction.h"

using namespace std;

namespace velox {

    void InterpreterFunctionDefinition::Execute(ScriptContext &sc) {
        // Intentionally left empty.
    }

    void InterpreterFunctionDefinition::Prepare(ScriptContext &sc) {
        if (sc.GetCurrentScope().HasItem(functionName)) {
            throw InterpreterError("Ambiguous function name '" + functionName + "'");
        }

        sc.GetCurrentScope().AddItem(functionName, CreateFunctionItem());
    }

    void InterpreterFunctionDefinition::SetFunction(std::shared_ptr<InterpreterFunctionScript> inFunction) {
        function = inFunction;
    }

    void InterpreterFunctionDefinition::SetFunctionName(const std::string &name) {
        functionName = name;
    }

    const std::string &InterpreterFunctionDefinition::GetFunctionName() const {
        return functionName;
    }

    std::shared_ptr<Item> InterpreterFunctionDefinition::CreateFunctionItem() const {
        return Item::Create(make_unique<ItemStateFunction>(function));
    }

}