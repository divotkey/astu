/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterVector : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param value the value of this literal
         */
        InterpreterVector() : InterpreterExpression(false) {}

        void SetXValueExpression(std::shared_ptr<InterpreterExpression> x);
        void SetYValueExpression(std::shared_ptr<InterpreterExpression> y);
        void SetZValueExpression(std::shared_ptr<InterpreterExpression> z);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> x;
        std::shared_ptr<InterpreterExpression> y;
        std::shared_ptr<InterpreterExpression> z;
    };

}


