// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterStatementWhile.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterStatementWhile::Execute(ScriptContext &sc) {
        sc.PushCodeBlockScope();

        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG);
        while (!sc.IsSet(ScriptContext::BREAK_EXECUTED_FLAG) && !sc.IsSet(ScriptContext::RETURN_EXECUTED_FLAG) &&
               condition->Evaluate(sc)->GetBooleanValue())
        {
            sc.ClearFlag(ScriptContext::CONTINUE_EXECUTED_FLAG);
            loopBody->Execute(sc);
        }
        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG | ScriptContext::CONTINUE_EXECUTED_FLAG);
        sc.PopLocalScope();
    }

    void InterpreterStatementWhile::SetCondition(std::shared_ptr<InterpreterExpression> inCondition) {
        condition = inCondition;
    }

    void InterpreterStatementWhile::SetStatement(std::shared_ptr<InterpreterStatement> statement) {
        loopBody = statement;
    }

}