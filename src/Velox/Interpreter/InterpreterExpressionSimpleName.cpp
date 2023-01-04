/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterExpressionSimpleName.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/ScriptContext.h"
#include "Velox/Interpreter/ItemStateUndefined.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterExpressionSimpleName::Evaluate(ScriptContext &sc) {
        if (IsLocation()) {

            auto result = sc.FindItem(name);
            if (!result) {
                result = Item::Create(make_unique<ItemStateUndefined>());
                if (sc.HasLocalScope()) {
                    sc.AddLocalItem(name, result);
                } else {
                    sc.AddGlobalItem(name, result);
                }
            }
            return result;
        } else {
            auto result = sc.FindItem(name);
            if (!result) {
                throw InterpreterError("Unknown identifier '" + name + "'", GetLineNumber());
            }
            return result;
        }
    }

} // end of namespace
