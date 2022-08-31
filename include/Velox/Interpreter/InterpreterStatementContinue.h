// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"

namespace velox {

    class InterpreterStatementContinue : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterStatementContinue(unsigned int lineNumber) : InterpreterStatement(lineNumber) {}

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;
    };

}



