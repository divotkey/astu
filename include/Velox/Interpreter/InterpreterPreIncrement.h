#pragma once

#include "InterpreterExpression.h"

namespace velox {

    class InterpreterPreIncrement : public InterpreterExpression {
    public:
        InterpreterPreIncrement(std::shared_ptr<InterpreterExpression> lValue, bool decrement = false)
            : value(lValue), decrement(decrement) {}

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



