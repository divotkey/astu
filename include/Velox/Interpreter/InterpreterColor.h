// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "InterpreterExpression.h"
#include "Graphics/Color.h"

namespace velox {

    class InterpreterColor : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param value the value of this literal
         */
        InterpreterColor() : InterpreterExpression(false) {}

        void SetRedExpression(std::shared_ptr<InterpreterExpression> red);
        void SetGreenExpression(std::shared_ptr<InterpreterExpression> green);
        void SetBlueExpression(std::shared_ptr<InterpreterExpression> blue);
        void SetAlphaExpression(std::shared_ptr<InterpreterExpression> alpha);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> red;
        std::shared_ptr<InterpreterExpression> green;
        std::shared_ptr<InterpreterExpression> blue;
        std::shared_ptr<InterpreterExpression> alpha;
    };

}


