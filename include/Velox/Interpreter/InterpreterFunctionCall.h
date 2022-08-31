// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

#include "Velox/Interpreter/InterpreterExpression.h"
#include "Velox/Interpreter/InterpreterActualParameterList.h"

namespace velox {

    class InterpreterFunctionCall : public InterpreterExpression {
    public:

        InterpreterFunctionCall(unsigned int lineNumber) : InterpreterExpression(lineNumber) {}

        void AddParameter(std::shared_ptr<InterpreterExpression> param);
        void SetFunction(std::shared_ptr<InterpreterExpression> inFunction);
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The expression which needs to be evaluated to receive the function item. */
        std::shared_ptr<InterpreterExpression> function;

        /** The actual parameters of this function call. */
        InterpreterExpressionParameterList parameters;
    };

}


