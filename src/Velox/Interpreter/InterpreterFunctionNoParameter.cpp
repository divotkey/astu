// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
