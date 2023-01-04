/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterFunctionTwoParameter.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {
    
    shared_ptr<Item> InterpreterFunctionTwoParameter::CreateItem(InterpreterFunctionTwoParameter::Func func) {
        return Item::CreateFunction(make_shared<InterpreterFunctionTwoParameter>(func));
    }

    InterpreterFunctionTwoParameter::InterpreterFunctionTwoParameter(Func func) : func(func)
    {
        AddFormalParameter("a");
        AddFormalParameter("b");
    }

    shared_ptr<Item> InterpreterFunctionTwoParameter::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        return func(sc, sc.FindItem("a"), sc.FindItem("b"), lineNumber);
    }

}
