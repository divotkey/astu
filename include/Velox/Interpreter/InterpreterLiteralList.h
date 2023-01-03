/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    class InterpreterLiteralList : public InterpreterExpression {
    public:

        /**
         * Constructor.
         */
        InterpreterLiteralList() : InterpreterExpression(false) {}

        void AddElement(std::shared_ptr<InterpreterExpression> elem);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The values of this literal. */
        std::vector<std::shared_ptr<InterpreterExpression>> elements;
    };

}


