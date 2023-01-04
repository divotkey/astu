/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include "InterpreterFunction.h"

namespace velox {

    class InterpreterFunctionPrint : public InterpreterFunction {
    public:

        InterpreterFunctionPrint();

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override;
    };

}



