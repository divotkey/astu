// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "InterpreterExpression.h"

namespace velox {

    class InterpreterLiteralBoolean : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param value the value of this literal
         */
        InterpreterLiteralBoolean(bool value) : InterpreterExpression(false), value(value) {}

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The value of this literal. */
        bool value;
    };

}

