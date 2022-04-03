#pragma once

#include "InterpreterExpression.h"
#include "InterpreterStatement.h"

#include <string>
#include <memory>

namespace velox {

    // Forward declaration.
    class InterpreterExpression;

    class InterpreterAssignment : public InterpreterExpression {
    public:

        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lValue);
        void SetRightHandSide(std::shared_ptr<InterpreterExpression> rValue);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The left-hand side (l-value or location) of this assignment. */
        std::shared_ptr<InterpreterExpression> lhs;

        /** The right-hand side (r-value or contents) of this assignment. */
        std::shared_ptr<InterpreterExpression> rhs;
    };

}



