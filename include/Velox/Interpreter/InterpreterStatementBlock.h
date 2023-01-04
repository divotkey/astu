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

    class InterpreterStatementBlock : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param loopBody      Whether the execution of statements ends, when continue or break is executed
         */
        InterpreterStatementBlock(bool loopBody) : loopBody(loopBody) {}

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

        /** Whether the execution of statements ends, when continue or break is executed. */
        bool loopBody;
    };

}


