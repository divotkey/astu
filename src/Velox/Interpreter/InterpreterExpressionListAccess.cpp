/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/InterpreterExpressionListAccess.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/ItemStateUndefined.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterExpressionListAccess::Evaluate(ScriptContext &sc) {
        auto leftItem = leftHandSide->Evaluate(sc);

        // Make sure item stays alive after evaluation of this member access; required for temporary objects
        sc.AddItem(leftItem);

        // Evaluate the index.
        auto index = indexExpr->Evaluate(sc);
        return leftItem->GetListElement(index->GetIntegerValue(), GetLineNumber());
    }

    void InterpreterExpressionListAccess::SetLeftHandSide(shared_ptr<InterpreterExpression> lhs) {
        leftHandSide = lhs;
    }

    void InterpreterExpressionListAccess::SetIndex(std::shared_ptr<InterpreterExpression> index) {
        indexExpr = index;
        indexExpr->SetLocation(false);
    }

}