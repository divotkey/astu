// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterClassDefinition.h"
#include "Velox/Interpreter/ItemStateFunction.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/InterpreterError.h"

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