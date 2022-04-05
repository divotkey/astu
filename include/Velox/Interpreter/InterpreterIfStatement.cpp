#include "InterpreterIfStatement.h"
#include "Item.h"

namespace velox {

    void InterpreterIfStatement::Execute(ScriptContext &sc) {
        if (condition->Evaluate(sc)->GetBooleanValue()) {
            thanPart->Execute(sc);
        } else if (elsePart) {
            elsePart->Execute(sc);
        }
    }

    void InterpreterIfStatement::SetCondition(std::shared_ptr<InterpreterExpression> inCondition) {
        condition = inCondition;
    }

    void InterpreterIfStatement::SetThenPart(std::shared_ptr<InterpreterStatement> statement) {
        thanPart = statement;
    }

    void InterpreterIfStatement::SetElsePart(std::shared_ptr<InterpreterStatement> statement) {
        elsePart = statement;
    }

}