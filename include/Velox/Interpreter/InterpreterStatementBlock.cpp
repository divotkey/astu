#include "InterpreterStatementBlock.h"
#include "Scope.h"

using namespace std;

namespace velox {

    void InterpreterStatementBlock::Execute(velox::ScriptContext &sc) {
        sc.PushScope(make_shared<Scope>());

        for (const auto &statement: statements) {
            statement->Execute(sc);
            if (sc.IsSet(ScriptContext::RETURN_EXECUTED_FLAG))
                break;
        }

        sc.PopScope();
    }

    void InterpreterStatementBlock::AddStatement(std::shared_ptr<InterpreterStatement> statement) {
        statements.push_back(statement);
    }

}

