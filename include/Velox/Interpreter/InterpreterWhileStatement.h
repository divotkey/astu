#pragma once

#include "InterpreterStatement.h"
#include "InterpreterExpression.h"

namespace velox {

    class InterpreterWhileStatement : public InterpreterStatement {
    public:
        void SetCondition(std::shared_ptr<InterpreterExpression> condition);
        void SetStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> condition;
        std::shared_ptr<InterpreterStatement> statement;
    };

}


