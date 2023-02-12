/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementSequence.h"
#include "Velox/Interpreter/Scope.h"

using namespace std;

namespace velox {

    void InterpreterStatementSequence::Execute(velox::ScriptContext &sc) {
        for (const auto &statement: statements) {
            statement->Prepare(sc);
        }

        for (const auto &statement: statements) {
            statement->Execute(sc);
            if (sc.IsFlagSet(ScriptContext::RETURN_EXECUTED_FLAG))
                break;
        }
    }

    void InterpreterStatementSequence::AddStatement(std::shared_ptr<InterpreterStatement> statement) {
        statements.push_back(statement);
    }

} // end of namespace

