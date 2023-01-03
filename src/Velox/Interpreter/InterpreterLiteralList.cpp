/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterLiteralList.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterLiteralList::Evaluate(ScriptContext &sc) {
        auto result = Item::CreateList();

        for (auto elem : elements) {
            result->AppendListElement(elem->Evaluate(sc));
        }

        return result;
    }

    void InterpreterLiteralList::AddElement(std::shared_ptr<InterpreterExpression> elem) {
        elements.push_back(elem);
    }

}