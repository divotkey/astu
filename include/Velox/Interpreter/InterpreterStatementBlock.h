// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/ScriptContext.h"
#include "Velox/Interpreter/InterpreterStatement.h"

// C++ Standard Library includes
#include <vector>
#include <memory>

namespace velox {

    class InterpreterStatementBlock : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param loopBody      Whether the execution of statements ends, when continue or break is executed
         */
        InterpreterStatementBlock(bool loopBody) : loopBody(loopBody) {}

        /**
         * Adds a statement to this statement block
         *
         * @param statement the statement to add
         */
        void AddStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterStatement
        void Execute(ScriptContext & sc) override;

    private:
        /** The statements this statement block contains. */
        std::vector<std::shared_ptr<InterpreterStatement>> statements;

        /** Whether the execution of statements ends, when continue or break is executed. */
        bool loopBody;
    };

}


