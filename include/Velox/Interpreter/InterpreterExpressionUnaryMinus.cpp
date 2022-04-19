// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterExpressionUnaryMinus.h"
#include "Item.h"

// C++ Standard Library
#include <cassert>

using namespace std;

namespace velox {

    InterpreterExpressionUnaryMinus::InterpreterExpressionUnaryMinus(unsigned int lineNumber,
                                                                     shared_ptr<InterpreterExpression> expr)
            : InterpreterExpression(lineNumber, false)
    {
        SetExpression(expr);
    }

    void InterpreterExpressionUnaryMinus::SetExpression(shared_ptr<InterpreterExpression> exp) {
        expression = exp;
        if (expression)
            expression->SetLocation(false);
    }

    shared_ptr<Item> velox::InterpreterExpressionUnaryMinus::Evaluate(velox::ScriptContext &sc) {
        assert(expression);
        auto result = expression->Evaluate(sc);
        return result->ExecuteUnaryMinus();
    }

}
