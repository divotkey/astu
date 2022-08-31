#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"
#include "Velox/Interpreter/InterpreterStatement.h"

// C++ Standard Library includes
#include <string>
#include <memory>

namespace velox {

    // Forward declaration.
    class InterpreterExpression;

    class InterpreterExpressionAssignment : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterExpressionAssignment(unsigned int lineNumber) : InterpreterExpression(lineNumber, false) {}

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



