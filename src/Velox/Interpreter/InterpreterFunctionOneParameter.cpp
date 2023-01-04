/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterFunctionOneParameter.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    std::shared_ptr<Item> InterpreterFunctionOneParameter::CreateItem(InterpreterFunctionOneParameter::Func func) {
        return Item::CreateFunction(make_shared<InterpreterFunctionOneParameter>(func));
    }

    InterpreterFunctionOneParameter::InterpreterFunctionOneParameter(Func func) : func(func)
    {
        AddFormalParameter("a");
    }

    std::shared_ptr<Item> InterpreterFunctionOneParameter::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        return func(sc, sc.FindItem("a"), lineNumber);
    }

}
