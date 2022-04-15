// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterStatementFor.h"
#include "Item.h"

namespace velox {

    void InterpreterStatementFor::Execute(ScriptContext &sc) {
        sc.PushScope();
        initStatement->Execute(sc);
        while (condition->Evaluate(sc)->GetBooleanValue()) {
            loopBody->Execute(sc);
            loopStatement->Execute(sc);
        }
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