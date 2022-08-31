// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterStatementBreak.h"

namespace velox {

    void InterpreterStatementBreak::Execute(ScriptContext &sc) {
        sc.SetFlag(ScriptContext::BREAK_EXECUTED_FLAG);
    }

}