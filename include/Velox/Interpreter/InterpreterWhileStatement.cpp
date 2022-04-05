#include "InterpreterWhileStatement.h"
#include "Item.h"

namespace velox {

    void InterpreterWhileStatement::Execute(ScriptContext &sc) {
        while (condition->Evaluate(sc)->GetBooleanValue()) {
            statement->Execute(sc);
        }
    }

    void InterpreterWhileStatement::SetCondition(std::shared_ptr<InterpreterExpression> inCondition) {
        condition = inCondition;
    }

    void InterpreterWhileStatement::SetStatement(std::shared_ptr<InterpreterStatement> inStatement) {
        statement = inStatement;
    }

}