#pragma once

// Local includes
#include "InterpreterActualParameterList.h"

namespace velox {

    // Forward declaration.
    class ScriptContext;
    class Item;

    class InterpreterConstructorCall {
    public:

        InterpreterConstructorCall(unsigned int lineNumber) : lineNumber(lineNumber) {}

        void AddParameter(std::shared_ptr<InterpreterExpression> param);
        void Execute(ScriptContext &sc, Item& function);

    private:
        /** The actual parameters of this function call. */
        InterpreterExpressionParameterList parameters;

        /** The line number of this constructor call. */
        unsigned int lineNumber;
    };

}


