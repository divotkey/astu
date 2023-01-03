/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterPostIncrement : public InterpreterExpression {
    public:
        InterpreterPostIncrement(std::shared_ptr<InterpreterExpression> lValue, bool decrement, unsigned int lineNumber)
            : InterpreterExpression(lineNumber, true), value(lValue), decrement(decrement) {}

        void SetValue(std::shared_ptr<InterpreterExpression> lValue);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The l-value of the increment. */
        std::shared_ptr<InterpreterExpression> value;

        /** Whether to decrement or increment the value by one. */
        bool decrement;
    };

}



