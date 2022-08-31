// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterPostIncrement : public InterpreterExpression {
    public:
        InterpreterPostIncrement(std::shared_ptr<InterpreterExpression> lValue, bool decrement, unsigned int lineNumber)
            : InterpreterExpression(lineNumber, true), value(lValue), decrement(decrement) {}

        void SetValue(std::shared_ptr<InterpreterExpression> lValue);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The l-value of the increment. */
        std::shared_ptr<InterpreterExpression> value;

        /** Whether to decrement or increment the value by one. */
        bool decrement;
    };

}



