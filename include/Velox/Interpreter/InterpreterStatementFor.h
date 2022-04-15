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

    class InterpreterStatementFor : public InterpreterStatement {
    public:
        void SetCondition(std::shared_ptr<InterpreterExpression> condition);
        void SetInitStatement(std::shared_ptr<InterpreterStatement> statement);
        void SetLoopStatement(std::shared_ptr<InterpreterStatement> statement);
        void SetLoopBody(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        /** The loopBody that will be executed once, before the loop starts. */
        std::shared_ptr<InterpreterStatement> initStatement;

        /** The conditional expression, that evaluated to continue or exit the loop. */
        std::shared_ptr<InterpreterExpression> condition;

        /** The loopBody that will be executed after each iteration of the loop. */
        std::shared_ptr<InterpreterStatement> loopStatement;

        /** The loopBody(s) that will be executed during each loop. */
        std::shared_ptr<InterpreterStatement> loopBody;
    };

}


