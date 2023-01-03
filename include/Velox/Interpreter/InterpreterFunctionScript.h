/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

#include "Velox/Interpreter/InterpreterStatementBlock.h"
#include "Velox/Interpreter/InterpreterFunction.h"
#include "Item.h"
#include "Velox/Interpreter/ScriptContext.h"
#include <memory>

namespace velox {

    class InterpreterFunctionScript : public InterpreterFunction {
    public:

        void SetStatement(std::shared_ptr<InterpreterStatement> statement);

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override;

    private:
        std::shared_ptr<InterpreterStatement> statement;
    };

}


