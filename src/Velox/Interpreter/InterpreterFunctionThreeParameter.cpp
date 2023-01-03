/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterFunctionThreeParameter.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {
    
    shared_ptr<Item> InterpreterFunctionThreeParameter::CreateItem(InterpreterFunctionThreeParameter::Func func) {
        return Item::CreateFunction(make_shared<InterpreterFunctionThreeParameter>(func));
    }

    InterpreterFunctionThreeParameter::InterpreterFunctionThreeParameter(Func func) : func(func)
    {
        AddFormalParameter("a");
        AddFormalParameter("b");
        AddFormalParameter("c");
    }

    shared_ptr<Item> InterpreterFunctionThreeParameter::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        return func(sc, sc.FindItem("a"), sc.FindItem("b"), sc.FindItem("c"), lineNumber);
    }

}
