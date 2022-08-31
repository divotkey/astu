// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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


