/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterFunctionDefinition.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateFunction.h"

using namespace std;

namespace velox {

    InterpreterFunctionDefinition::InterpreterFunctionDefinition(unsigned int lineNumber)
        : InterpreterStatement(lineNumber)
    {
        // Intentionally left empty
    }

    void InterpreterFunctionDefinition::Execute(ScriptContext &sc) {
        // Intentionally left empty.
    }

    void InterpreterFunctionDefinition::Prepare(ScriptContext &sc) {

        if (sc.HasLocalItem(functionName) || sc.HasGlobalItem(functionName)){
            throw InterpreterError("Ambiguous function name '" + functionName + "'", GetLineNumber());
        }

        if (sc.HasLocalScope()) {
            sc.AddLocalItem(functionName, CreateFunctionItem());
        } else {
            sc.AddGlobalItem(functionName, CreateFunctionItem());
        }

        // TODO remove commented out legacy code, when refactoring is done.
        // Code used before refactoring.
        //if (sc.GetCurrentScope().HasItem(functionName)) {
        //    throw InterpreterError("Ambiguous function name '" + functionName + "'");
        //}
        //
        //sc.GetCurrentScope().AddItem(functionName, CreateFunctionItem());
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