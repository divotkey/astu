// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "InterpreterStatement.h"
#include "InterpreterExpression.h"

namespace velox {

    class InterpreterStatementIf : public InterpreterStatement {
    public:
        void SetCondition(std::shared_ptr<InterpreterExpression> condition);
        void SetThenPart(std::shared_ptr<InterpreterStatement> statement);
        void SetElsePart(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> condition;
        std::shared_ptr<InterpreterStatement> thanPart;
        std::shared_ptr<InterpreterStatement> elsePart;
    };

}


