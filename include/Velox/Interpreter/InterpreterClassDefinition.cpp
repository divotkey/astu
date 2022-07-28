// Local includes
#include "InterpreterClassDefinition.h"
#include "ItemStateFunction.h"
#include "ObjectType.h"
#include "InterpreterError.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace velox {

    void InterpreterClassDefinition::Execute(ScriptContext &sc) {
        // Intentionally left empty.
    }

    void InterpreterClassDefinition::Prepare(ScriptContext &sc) {
        if (sc.HasObjectType(typeName)) {
            throw InterpreterError("Ambiguous class name '" + typeName + "'");
        }

        auto type = std::make_shared<ObjectType>();
        for (auto &function : functions) {
            type->AddItem(function->GetFunctionName(), function->CreateFunctionItem());
        }

        sc.AddObjectType(typeName, type);
    }

    bool InterpreterClassDefinition::HasFunction(const std::string &name) const {
        for (auto const &function : functions) {
            if (function->GetFunctionName() == name)
                return true;
        }

        return false;
    }

    void InterpreterClassDefinition::AddFunction(std::shared_ptr<InterpreterFunctionDefinition> function) {
        assert(!HasFunction(function->GetFunctionName()));
        functions.push_back(function);
    }

}