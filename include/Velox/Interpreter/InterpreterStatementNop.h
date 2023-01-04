/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    /**
     * No-Operation statement.
     *
     * This statement is useful for empty for-loop init or loop statements.
     */
    class InterpreterStatementNop : public InterpreterStatement {
    public:

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;
    };

} // end of namespace


