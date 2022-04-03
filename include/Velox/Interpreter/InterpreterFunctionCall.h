#pragma once

#include "InterpreterExpression.h"
#include "InterpreterActualParameterList.h"

namespace velox {

    class InterpreterFunctionCall : public InterpreterExpression {
    public:

        InterpreterFunctionCall() : InterpreterExpression(false) {}

        void AddParameter(std::shared_ptr<InterpreterExpression> param);
        void SetFunction(std::shared_ptr<InterpreterExpression> inFunction);
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The expression which needs to be evaluated to receive the function item. */
        std::shared_ptr<InterpreterExpression> function;

        /** The actual parameters of this function call. */
        InterpreterActualParameterList parameters;
    };

}


