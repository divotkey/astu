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

    class InterpreterLiteralString : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param value the value of this literal
         */
        InterpreterLiteralString(const std::string &value) : InterpreterExpression(false), value(value) {}

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The value of this literal. */
        std::string value;
    };

}


