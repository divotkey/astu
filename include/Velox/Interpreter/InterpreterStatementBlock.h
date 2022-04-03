#pragma once

#include "ScriptContext.h"
#include "InterpreterStatement.h"
#include <vector>
#include <memory>

namespace velox {

    class InterpreterStatementBlock : public InterpreterStatement {
    public:

        void AddStatement(std::shared_ptr<InterpreterStatement> statement);

        void Execute(ScriptContext & sc) override;

    private:
        std::vector<std::shared_ptr<InterpreterStatement>> statements;
    };

}


