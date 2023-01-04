/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementFor.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterStatementFor::Execute(ScriptContext &sc) {
        sc.PushCodeBlockScope();
        initStatement->Execute(sc);

        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG);
        while (!sc.IsFlagSet(ScriptContext::BREAK_EXECUTED_FLAG) && !sc.IsFlagSet(ScriptContext::RETURN_EXECUTED_FLAG) &&
               condition->Evaluate(sc)->GetBooleanValue())
        {
            sc.ClearFlag(ScriptContext::CONTINUE_EXECUTED_FLAG);
            loopBody->Execute(sc);
            loopStatement->Execute(sc);
        }
        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG | ScriptContext::CONTINUE_EXECUTED_FLAG);
        sc.PopLocalScope();
    }

    void InterpreterStatementFor::SetCondition(std::shared_ptr<InterpreterExpression> inCondition) {
        condition = inCondition;
    }

    void InterpreterStatementFor::SetInitStatement(std::shared_ptr<InterpreterStatement> statement) {
        initStatement = statement;
    }

    void InterpreterStatementFor::SetLoopStatement(std::shared_ptr<InterpreterStatement> statement) {
        loopStatement = statement;
    }

    void InterpreterStatementFor::SetLoopBody(std::shared_ptr<InterpreterStatement> statement) {
        loopBody = statement;
    }

}