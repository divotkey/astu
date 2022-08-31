// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterActualParameterList.h"

namespace velox {

    // Forward declaration.
    class ScriptContext;
    class Item;

    class InterpreterConstructorCall {
    public:

        InterpreterConstructorCall(unsigned int lineNumber) : lineNumber(lineNumber) {}

        void AddParameter(std::shared_ptr<InterpreterExpression> param);
        void Execute(ScriptContext &sc, Item& function);

    private:
        /** The actual parameters of this function call. */
        InterpreterExpressionParameterList parameters;

        /** The line number of this constructor call. */
        unsigned int lineNumber;
    };

}


