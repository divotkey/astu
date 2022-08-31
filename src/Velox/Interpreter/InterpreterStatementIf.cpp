// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterStatementIf.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterStatementIf::Execute(ScriptContext &sc) {
        if (condition->Evaluate(sc)->GetBooleanValue()) {
            thanPart->Execute(sc);
        } else if (elsePart) {
            elsePart->Execute(sc);
        }
    }

    void InterpreterStatementIf::SetCondition(std::shared_ptr<InterpreterExpression> inCondition) {
        condition = inCondition;
    }

    void InterpreterStatementIf::SetThenPart(std::shared_ptr<InterpreterStatement> statement) {
        thanPart = statement;
    }

    void InterpreterStatementIf::SetElsePart(std::shared_ptr<InterpreterStatement> statement) {
        elsePart = statement;
    }

}