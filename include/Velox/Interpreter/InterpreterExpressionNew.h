#pragma once

// Local includes
#include "InterpreterExpression.h"
#include "InterpreterConstructorCall.h"

// C++ Standard Library includes
#include <string>

namespace velox {

    class InterpreterExpressionNew : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterExpressionNew(unsigned int lineNumber) : InterpreterExpression(lineNumber) {}

        void SetTypeName(const std::string& name);
        void SetConstructorCall(std::shared_ptr<InterpreterConstructorCall> call);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::string typeName;
        std::shared_ptr<InterpreterConstructorCall> constructorCall;
    };

}



