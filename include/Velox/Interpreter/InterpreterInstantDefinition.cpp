// Local includes
#include "InterpreterInstantDefinition.h"
#include "ObjectType.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace velox {

    bool InterpreterInstantDefinition::HasFunction(const std::string &name) const {
        for (auto const &function : functions) {
            if (function->GetFunctionName() == name)
                return true;
        }

        return false;
    }

    void InterpreterInstantDefinition::AddFunction(std::shared_ptr<InterpreterFunctionDefinition> function) {
        assert(!HasFunction(function->GetFunctionName()));
        functions.push_back(function);
    }

    void InterpreterInstantDefinition::Prepare(ScriptContext &sc) {
        if (sc.HasObjectType(typeName)) {
            throw InterpreterError("Ambiguous class name '" + typeName + "'");
        }

        auto type = std::make_shared<ObjectType>();
        for (auto &function : functions) {
            type->AddItem(function->GetFunctionName(), function->CreateFunctionItem());
        }

        sc.AddObjectType(typeName, type);
    }

    void InterpreterInstantDefinition::Execute(ScriptContext &sc) {
        // Intentionally left empty.
    }
}