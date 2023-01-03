/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementContinue.h"

namespace velox {

    void InterpreterStatementContinue::Execute(ScriptContext &sc) {
        sc.SetFlag(ScriptContext::CONTINUE_EXECUTED_FLAG);
    }

}