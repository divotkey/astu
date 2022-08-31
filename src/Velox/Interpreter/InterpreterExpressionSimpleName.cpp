// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
            auto result = sc.FindLocalItem(name);
            if (!result) {
                result = Item::Create(make_unique<ItemStateUndefined>());
                sc.AddItem(name, result);
            }
            return result;
        } else {
            auto result = sc.FindItem(name);
            if (!result) {
                throw InterpreterError("Unknown identifier '" + name + "'", GetLineNumber());
            }
            return result;
        }

        //auto result = sc.FindItem(name);
        //if (!result) {
        //    if (!IsLocation()) {
        //        throw InterpreterError("Unknown identifier '" + name + "'", GetLineNumber());
        //    }
        //
        //    result = Item::Create(make_unique<ItemStateUndefined>());
        //    sc.AddItem(name, result);
        //}
        //
        //
        //return result;
    }

}
