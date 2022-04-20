// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterFunctionThreeParameter.h"
#include "Item.h"

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
