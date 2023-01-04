/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

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
            sc.PushFunctionScope();
            constructor->CallAsFunction(sc, noParams, GetLineNumber());
            sc.PopLocalScope();
        }

        // Run statements.
        auto memberScope = std::make_shared<Scope>();
        result->AddItemsToScope(*memberScope);

        sc.PushCodeBlockScope(memberScope);
        sc.PushInstant(result);
        for (auto statement : statements) {
            statement->Execute(sc);
        }
        sc.PopInstant();
        sc.PopLocalScope();

        // Realize instant.
        auto realize = result->FindItem("realize");
        if (realize) {
            if (sc.HasInstant()) {
                result->AddItem("parent", sc.PeekInstant());
            } else {
                result->AddItem("parent", Item::CreateUndefined());
            }
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