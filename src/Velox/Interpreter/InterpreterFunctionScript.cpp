/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterFunctionScript.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateUndefined.h"

using namespace std;

namespace velox {

    std::shared_ptr<Item> InterpreterFunctionScript::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        sc.PushReturnValue();
        statement->Execute(sc);
        sc.ClearFlag(ScriptContext::RETURN_EXECUTED_FLAG);

        auto result = sc.PopReturnValue();
        if (!result)
            result = Item::Create(make_unique<ItemStateUndefined>());

        return result;
    }

    void InterpreterFunctionScript::SetStatement(std::shared_ptr<InterpreterStatement> inStatement) {
        statement = inStatement;
    }

}

