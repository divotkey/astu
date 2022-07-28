// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "InterpreterExpression.h"
#include "Operators.h"

namespace velox {

    class InterpreterExpressionUnaryMinus : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterExpressionUnaryMinus(unsigned int lineNumber, std::shared_ptr<InterpreterExpression> expr = nullptr);

        void SetExpression(std::shared_ptr<InterpreterExpression> expr);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> expression;
    };

}


