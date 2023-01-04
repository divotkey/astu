/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include "Velox/Interpreter/InterpreterStatement.h"
#include "InterpreterFunctionScript.h"
#include "Velox/Interpreter/InterpreterExpressionSimpleName.h"

namespace velox {

    class InterpreterFunctionDefinition : public InterpreterStatement {
    public:

        InterpreterFunctionDefinition(unsigned int lineNumber);

        void SetFunction(std::shared_ptr<InterpreterFunctionScript> function);
        void SetFunctionName(const std::string& name);
        const std::string& GetFunctionName() const;

        std::shared_ptr<Item> CreateFunctionItem() const;

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

        void Prepare(ScriptContext &sc) override;

    private:
        std::shared_ptr<InterpreterFunctionScript> function;
        std::string functionName;
    };

}



