// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterInstantRealization.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    InterpreterInstantRealization::InterpreterInstantRealization(unsigned int lineNumber)
        : InterpreterExpression(lineNumber)
    {
        // Intentionally left empty.
    }

    std::shared_ptr<Item> InterpreterInstantRealization::Evaluate(ScriptContext &sc) {
        auto type = sc.FindObjectType(typeName);
        if (!type) {
            throw InterpreterError("Unknown object type '" + typeName + "'", GetLineNumber());
        }

        // TODO improve this memory usage.
        InterpreterNoParameterList noParams;

        // Call constructor of instant
        auto result = type->CreateObject(sc);
        auto constructor = result->FindItem(typeName);
        if (constructor) {
            sc.PushScope();
            constructor->CallAsFunction(sc, noParams, GetLineNumber());
            sc.PopScope();
        }

        // Run statements.
        sc.PushScope();
        result->AddItemsToScope(sc);
        for (auto statement : statements) {
            statement->Execute(sc);
        }
        sc.PopScope();

        // Realize instant.
        auto realize = result->FindItem("realize");
        if (realize) {
            realize->CallAsFunction(sc, noParams, GetLineNumber());
        }

        return result;
    }

    void InterpreterInstantRealization::AddStatement(std::shared_ptr<InterpreterStatement> statement) {
        statements.push_back(statement);
    }

    void InterpreterInstantRealization::SetTypeName(const std::string &name) {
        typeName = name;
    }

}