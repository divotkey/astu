// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterConstructorCall.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterConstructorCall::AddParameter(std::shared_ptr<InterpreterExpression> param) {
        parameters.AddParameter(param);
    }

    void InterpreterConstructorCall::Execute(ScriptContext &sc, Item &function) {
        sc.PushScope();
        function.CallAsFunction(sc, parameters, lineNumber);
        sc.PopScope();
    }
}
