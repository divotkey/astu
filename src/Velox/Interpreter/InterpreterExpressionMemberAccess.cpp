/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/InterpreterExpressionMemberAccess.h"
#include "Velox/Interpreter/ItemStateUndefined.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterExpressionMemberAccess::Evaluate(ScriptContext &sc) {
        auto leftItem = leftHandSide->Evaluate(sc);

        // Make sure item stays alive after evaluation of this member access; required for temporary objects
        sc.AddItem(leftItem);

        auto result = leftItem->FindItem(name);

        if (!result) {
            if (!IsLocation()) {
                throw InterpreterError("Unknown field '" + name + "'", GetLineNumber());
            }
            result = Item::Create(make_unique<ItemStateUndefined>());
            leftItem->AddItem(name, result);
        }
        return result;
    }

    void InterpreterExpressionMemberAccess::SetLeftHandSide(shared_ptr<InterpreterExpression> lhs) {
        leftHandSide = lhs;
    }

    void InterpreterExpressionMemberAccess::SetRightHandSide(const string &inName) {
        name = inName;
    }

}