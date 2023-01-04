/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterExpressionNew.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"

// C++ Standard Library includes
#include <cassert>

namespace velox {

    std::shared_ptr<Item> InterpreterExpressionNew::Evaluate(ScriptContext &sc) {
        auto type = sc.FindObjectType(typeName);
        if (!type) {
            throw InterpreterError("Unknown object type '" + typeName + "'", GetLineNumber());
        }

        auto result = type->CreateObject(sc);
        assert(constructorCall);

        if (result->HasItem(typeName)) {
            constructorCall->Execute(sc, result->GetItem(typeName));
        } else if (constructorCall->NumParameters() > 0) {
            throw InterpreterError("No matching constructor signature found for '" + typeName + "'", GetLineNumber());
        }

        return result;
    }

    void InterpreterExpressionNew::SetConstructorCall(std::shared_ptr<InterpreterConstructorCall> call) {
        constructorCall = call;
    }

    void InterpreterExpressionNew::SetTypeName(const std::string &name) {
        typeName = name;
    }

}