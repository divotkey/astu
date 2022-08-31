// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterStatementFor.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterStatementFor::Execute(ScriptContext &sc) {
        sc.PushScope();
        initStatement->Execute(sc);

        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG);
        while (!sc.IsSet(ScriptContext::BREAK_EXECUTED_FLAG) && !sc.IsSet(ScriptContext::RETURN_EXECUTED_FLAG) &&
               condition->Evaluate(sc)->GetBooleanValue())
        {
            sc.ClearFlag(ScriptContext::CONTINUE_EXECUTED_FLAG);
            loopBody->Execute(sc);
            loopStatement->Execute(sc);
        }
        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG | ScriptContext::CONTINUE_EXECUTED_FLAG);
        sc.PopScope();
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