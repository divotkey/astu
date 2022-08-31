// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterReturnStatement : public InterpreterStatement {
    public:

        void SetReturnExpression(std::shared_ptr<InterpreterExpression> expression);
        void Execute(ScriptContext &sc) override;


    private:
        /** The expression that gets evaluated and becomes the return value. */
        std::shared_ptr<InterpreterExpression> expression;
    };

}


