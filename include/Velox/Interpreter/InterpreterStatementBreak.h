/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"

namespace velox {

    class InterpreterStatementBreak : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterStatementBreak(unsigned int lineNumber) : InterpreterStatement(lineNumber) {}

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;
    };

}



