/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"

// C++ Standard Library
#include <string>

namespace velox {

    class InterpreterExpressionMemberAccess : public InterpreterExpression {
    public:

        InterpreterExpressionMemberAccess(unsigned int lineNumber) : InterpreterExpression(lineNumber, true) { }

        void SetLeftHandSide(std::shared_ptr<InterpreterExpression> lhs);
        void SetRightHandSide(const std::string& name);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterExpression> leftHandSide;
        std::string name;
    };

}


