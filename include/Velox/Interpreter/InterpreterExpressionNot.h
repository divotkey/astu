/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterExpressionNot : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param expr          the logical expression that should be negated
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterExpressionNot(std::shared_ptr<InterpreterExpression> expr, unsigned int lineNumber);

        /**
         * Sets the logical expression to be negated.
         *
         * @param inExpression  the logical expression
         */
        void SetExpression(std::shared_ptr<InterpreterExpression> inExpression);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> expression;
    };

} // end of namespace



