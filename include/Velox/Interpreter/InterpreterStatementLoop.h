// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"

namespace velox {

    class InterpreterStatementLoop : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterStatementLoop(unsigned int lineNumber);
        void SetStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext &sc) override;

    private:
        /** The body of this loop. */
        std::shared_ptr<InterpreterStatement> loopBody;
    };

}


