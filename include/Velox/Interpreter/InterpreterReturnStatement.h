#pragma once

#include "InterpreterStatement.h"
#include "InterpreterExpression.h"

namespace velox {

    class InterpreterReturnStatement : public InterpreterStatement {
    public:

        void SetReturnExpression(std::shared_ptr<InterpreterExpression> expression);
        void Execute(ScriptContext &sc) override;


    private:
        /** The expression that gets evaluated and becomes the return value. */
        std::shared_ptr<InterpreterExpression> expression;
    };

}


