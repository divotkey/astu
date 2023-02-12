/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/ScriptContext.h"
#include "Velox/Interpreter/InterpreterStatement.h"

// C++ Standard Library includes
#include <vector>
#include <memory>

namespace velox {

    class InterpreterStatementSequence : public InterpreterStatement {
    public:

        /**
         * Constructor.
         */
        InterpreterStatementSequence() = default;

        /**
         * Adds a statement to this sequence
         *
         * @param statement the statement to add
         */
        void AddStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext & sc) override;

    private:
        /** The statements of this sequence. */
        std::vector<std::shared_ptr<InterpreterStatement>> statements;
    };

} // end of namespace


