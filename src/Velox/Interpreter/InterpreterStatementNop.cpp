/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementNop.h"

namespace velox {

    void InterpreterStatementNop::Execute(ScriptContext &sc) {
        return;
    }

} // end of namespace