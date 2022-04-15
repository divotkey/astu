// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterFunctionTwoParameter.h"

using namespace std;

namespace velox {

    InterpreterFunctionTwoParameter::InterpreterFunctionTwoParameter(Func func) : func(func)
    {
        AddFormalParameter("a");
        AddFormalParameter("b");
    }

    std::shared_ptr<Item> InterpreterFunctionTwoParameter::DoEvaluate(ScriptContext &sc, unsigned int lineNumber) {
        return func(sc.FindItem("a"), sc.FindItem("b"), lineNumber);
    }

}
