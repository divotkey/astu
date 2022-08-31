// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    void InterpreterExpression::Execute(ScriptContext &sc) {
        // Evaluate this expression and ignore the result.
        Evaluate(sc);
    }

}
