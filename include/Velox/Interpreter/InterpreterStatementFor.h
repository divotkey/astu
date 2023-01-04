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

    class InterpreterStatementFor : public InterpreterStatement {
    public:
        void SetCondition(std::shared_ptr<InterpreterExpression> condition);
        void SetInitStatement(std::shared_ptr<InterpreterStatement> statement);
        void SetLoopStatement(std::shared_ptr<InterpreterStatement> statement);
        void SetLoopBody(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        /** The loopBody that will be executed once, before the loop starts. */
        std::shared_ptr<InterpreterStatement> initStatement;

        /** The conditional expression, that evaluated to continue or exit the loop. */
        std::shared_ptr<InterpreterExpression> condition;

        /** The loopBody that will be executed after each iteration of the loop. */
        std::shared_ptr<InterpreterStatement> loopStatement;

        /** The loopBody(s) that will be executed during each loop. */
        std::shared_ptr<InterpreterStatement> loopBody;
    };

}


