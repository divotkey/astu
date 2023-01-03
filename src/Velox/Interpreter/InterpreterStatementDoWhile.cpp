/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementDoWhile.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterStatementDoWhile::Execute(ScriptContext &sc) {
        sc.PushCodeBlockScope();

        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG);
        do {
            sc.ClearFlag(ScriptContext::CONTINUE_EXECUTED_FLAG);
            loopBody->Execute(sc);
        } while (!sc.IsFlagSet(ScriptContext::BREAK_EXECUTED_FLAG) && !sc.IsFlagSet(ScriptContext::RETURN_EXECUTED_FLAG) &&
                 condition->Evaluate(sc)->GetBooleanValue());

        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG | ScriptContext::CONTINUE_EXECUTED_FLAG);
        sc.PopLocalScope();
    }

    void InterpreterStatementDoWhile::SetCondition(std::shared_ptr<InterpreterExpression> inCondition) {
        condition = inCondition;
    }

    void InterpreterStatementDoWhile::SetStatement(std::shared_ptr<InterpreterStatement> statement) {
        loopBody = statement;
    }

}