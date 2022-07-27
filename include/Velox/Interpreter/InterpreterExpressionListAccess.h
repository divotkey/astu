#pragma once

// Local includes
#include "InterpreterExpression.h"

// C++ Standard Library
#include <string>

namespace velox {

    class InterpreterExpressionListAccess : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterExpressionListAccess(unsigned int lineNumber) : InterpreterExpression(lineNumber, true) { }

        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lhs);
        void SetIndex(std::shared_ptr<InterpreterExpression> index);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> leftHandSide;
        std::shared_ptr<InterpreterExpression> indexExpr;
    };

}


