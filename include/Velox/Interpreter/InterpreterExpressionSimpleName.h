// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"

// C++ Standard Library includes
#include <string>

namespace velox {

    class InterpreterExpressionSimpleName : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param name  the name this expression represents
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterExpressionSimpleName(const std::string & name, unsigned int lineNumber)
            : InterpreterExpression(lineNumber, true), name(name) {}

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The name this expression represents. */
        std::string name;
    };

}


