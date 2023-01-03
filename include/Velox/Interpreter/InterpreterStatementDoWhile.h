/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterStatementDoWhile : public InterpreterStatement {
    public:

        /**
         * Constructor
         *
         * @param lineNumber    information about the location within the source code
         */
        InterpreterStatementDoWhile(unsigned int lineNumber) : InterpreterStatement(lineNumber) {}

        void SetCondition(std::shared_ptr<InterpreterExpression> condition);
        void SetStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> condition;
        std::shared_ptr<InterpreterStatement> loopBody;
    };

}


