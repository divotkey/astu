// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterLiteralList.h"
#include "Item.h"

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