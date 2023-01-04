/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterLiteralString.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateString.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterLiteralString::Evaluate(ScriptContext &sc) {
        return Item::CreateString(value);
    }

}
