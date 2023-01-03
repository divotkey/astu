/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterReturnStatement : public InterpreterStatement {
    public:

        void SetReturnExpression(std::shared_ptr<InterpreterExpression> expression);
        void Execute(ScriptContext &sc) override;


    private:
        /** The expression that gets evaluated and becomes the return value. */
        std::shared_ptr<InterpreterExpression> expression;
    };

}


