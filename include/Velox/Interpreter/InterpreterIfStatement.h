#pragma once

#include "InterpreterStatement.h"
#include "InterpreterExpression.h"

namespace velox {

    class InterpreterIfStatement : public InterpreterStatement {
    public:
        void SetCondition(std::shared_ptr<InterpreterExpression> condition);
        void SetThenPart(std::shared_ptr<InterpreterStatement> statement);
        void SetElsePart(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> condition;
        std::shared_ptr<InterpreterStatement> thanPart;
        std::shared_ptr<InterpreterStatement> elsePart;
    };

}


