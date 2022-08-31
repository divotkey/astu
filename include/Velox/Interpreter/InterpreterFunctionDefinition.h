// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

#include "Velox/Interpreter/InterpreterStatement.h"
#include "InterpreterFunctionScript.h"
#include "Velox/Interpreter/InterpreterExpressionSimpleName.h"

namespace velox {

    class InterpreterFunctionDefinition : public InterpreterStatement {
    public:

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



