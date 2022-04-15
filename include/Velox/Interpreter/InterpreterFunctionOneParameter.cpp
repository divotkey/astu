// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterFunctionOneParameter.h"

using namespace std;

namespace velox {

    InterpreterFunctionOneParameter::InterpreterFunctionOneParameter(Func func) : func(func)
    {
        AddFormalParameter("a");
    }

    std::shared_ptr<Item> InterpreterFunctionOneParameter::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        return func(sc.FindItem("a"), lineNumber);
    }

}
