// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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