/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/ScriptContext.h"
#include "Velox/Interpreter/InterpreterStatement.h"

// C++ Standard Library includes
#include <vector>
#include <memory>

namespace velox {

    class InterpreterScript : public InterpreterStatement {
    public:

        /**
         * Constructor.
         */
        InterpreterScript() = default;

        /**
         * Adds a statement to this statement block
         *
         * @param statement the statement to add
         */
        void AddStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext & sc) override;

    private:
        /** The statements this statement block contains. */
        std::vector<std::shared_ptr<InterpreterStatement>> statements;
    };

}


