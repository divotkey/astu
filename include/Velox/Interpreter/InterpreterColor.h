/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"
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


