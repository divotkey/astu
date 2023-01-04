/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementBreak.h"

namespace velox {

    void InterpreterStatementBreak::Execute(ScriptContext &sc) {
        sc.SetFlag(ScriptContext::BREAK_EXECUTED_FLAG);
    }

}