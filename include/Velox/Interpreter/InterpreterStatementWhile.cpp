// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterStatementWhile.h"
#include "Item.h"

namespace velox {

    void InterpreterStatementWhile::Execute(ScriptContext &sc) {
        sc.PushScope();
        while (condition->Evaluate(sc)->GetBooleanValue()) {
            loopBody->Execute(sc);
        }
        sc.PopScope();
    }

    void InterpreterStatementWhile::SetCondition(std::shared_ptr<InterpreterExpression> inCondition) {
        condition = inCondition;
    }

    void InterpreterStatementWhile::SetStatement(std::shared_ptr<InterpreterStatement> statement) {
        loopBody = statement;
    }

}