// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterExpression.h"

namespace velox {

    /**
     * No-Operation statement.
     */
    class InterpreterStatementNop : public InterpreterStatement {
    public:

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;
    };

}


