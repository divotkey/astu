/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementBlock.h"
#include "Velox/Interpreter/Scope.h"

using namespace std;

namespace velox {

    void InterpreterStatementBlock::Execute(velox::ScriptContext &sc) {
        sc.PushCodeBlockScope(make_shared<Scope>());

        for (const auto &statement: statements) {
            statement->Prepare(sc);
        }

        for (const auto &statement: statements) {
            statement->Execute(sc);
            if (sc.IsFlagSet(ScriptContext::RETURN_EXECUTED_FLAG))
                break;

            if (loopBody &&
                (sc.IsFlagSet(ScriptContext::BREAK_EXECUTED_FLAG) || sc.IsFlagSet(ScriptContext::CONTINUE_EXECUTED_FLAG)))
                break;
        }

        sc.PopLocalScope();
    }

    void InterpreterStatementBlock::AddStatement(std::shared_ptr<InterpreterStatement> statement) {
        statements.push_back(statement);
    }

}

