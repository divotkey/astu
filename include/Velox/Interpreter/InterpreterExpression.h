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
         * @param location  whether this expression represents an l-value
         */
        InterpreterExpression(bool location = false) : location(location) {}

        /** Virtual destructor. */
        ~InterpreterExpression() {}

        /**
         * Whether this expression represents an location (l-value) of an variable.
         *
         * @return
         */
        bool IsLocation() const {
            return location;
        }

        /**
         * Defines this expression represents an location (l-value) of an variable.
         *
         * @param b `true` to make this an l-value
         */
        void SetLocation(bool b) {
            location = b;
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

    private:
        /** Whether this expression represents a location (l-Value). */
        bool location;
    };

}


