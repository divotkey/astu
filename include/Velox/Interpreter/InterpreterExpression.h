/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include "InterpreterStatement.h"
#include <memory>

namespace velox {

    // Forward declaration
    class ScriptContext;
    class Item;

    class InterpreterExpression : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the location within the source code
         * @param locator       whether this expression represents a lvalue
         */
        InterpreterExpression(unsigned int lineNumber = 0, bool locator = false)
            : InterpreterStatement(lineNumber), locator(locator) {}

        /**
         * Whether this expression represents an location (l-value) of an variable.
         *
         * @return
         */
        bool IsLocation() const {
            return locator;
        }

        /**
         * Defines this expression represents an location (l-value) of an variable.
         *
         * @param b `true` to make this an l-value
         */
        void SetLocation(bool b) {
            locator = b;
        }

        /**
         * Evaluates this expression.
         *
         * @param sc    the script context used to evaluate this expression
         * @return the item (value) of this expression
         */
        virtual std::shared_ptr<Item> Evaluate(ScriptContext &sc) = 0;

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    protected:
        /** Whether this expression represents a lvalue. */
        bool locator;
    };

}


