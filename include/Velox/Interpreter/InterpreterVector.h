// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "InterpreterExpression.h"

namespace velox {

    class InterpreterVector : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param value the value of this literal
         */
        InterpreterVector() : InterpreterExpression(false) {}

        void SetXValueExpression(std::shared_ptr<InterpreterExpression> x);
        void SetYValueExpression(std::shared_ptr<InterpreterExpression> y);
        void SetZValueExpression(std::shared_ptr<InterpreterExpression> z);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> x;
        std::shared_ptr<InterpreterExpression> y;
        std::shared_ptr<InterpreterExpression> z;
    };

}


