/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterExpressionUndefined.h"
#include "Velox/Interpreter/Item.h"

// C++ Standard Library
#include <cassert>

using namespace std;

namespace velox {

    shared_ptr<Item> velox::InterpreterExpressionUndefined::Evaluate(velox::ScriptContext &sc) {
        return Item::CreateUndefined();
    }

}
