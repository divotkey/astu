/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterLiteralBoolean : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param value the value of this literal
         */
        InterpreterLiteralBoolean(bool value) : InterpreterExpression(false), value(value) {}

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The value of this literal. */
        bool value;
    };

}


