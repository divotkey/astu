/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterFunctionNoParameter.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterFunctionNoParameter::CreateItem(InterpreterFunctionNoParameter::Func func) {
        return Item::CreateFunction(make_shared<InterpreterFunctionNoParameter>(func));
    }

    InterpreterFunctionNoParameter::InterpreterFunctionNoParameter(Func func) : func(func)
    {
        // Intentionally left empty.
    }

    shared_ptr<Item> InterpreterFunctionNoParameter::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        return func(lineNumber);
    }


}
