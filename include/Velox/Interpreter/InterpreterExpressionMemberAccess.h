// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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


