/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterInstantDefinition.h"
#include "Velox/Interpreter/ObjectType.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace velox {

    InterpreterInstantDefinition::InterpreterInstantDefinition(const string &name, unsigned int lineNumber)
            : InterpreterStatement(lineNumber), typeName(name)
    {
        // Intentionally left empty.
    }

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

    const std::string &InterpreterInstantDefinition::GetTypeName() const
    {
        return typeName;
    }

    std::shared_ptr<ObjectType> InterpreterInstantDefinition::CreateObjectType() const
    {
        auto type = std::make_shared<ObjectType>();
        for (auto &function : functions) {
            type->AddItem(function->GetFunctionName(), function->CreateFunctionItem());
        }

        return type;
    }

    void InterpreterInstantDefinition::Prepare(ScriptContext &sc) {
        if (sc.HasObjectType(typeName)) {
            throw InterpreterError("Ambiguous class or instant name '" + typeName + "'", GetLineNumber());
        }

        sc.AddObjectType(typeName, CreateObjectType());
    }

    void InterpreterInstantDefinition::Execute(ScriptContext &sc) {
        // Intentionally left empty.
    }

}   // end of namespace