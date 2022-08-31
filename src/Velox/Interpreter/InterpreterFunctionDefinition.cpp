// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/InterpreterFunctionDefinition.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateFunction.h"

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