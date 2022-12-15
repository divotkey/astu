// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
            if (sc.IsSet(ScriptContext::RETURN_EXECUTED_FLAG))
                break;

            if (loopBody &&
                (sc.IsSet(ScriptContext::BREAK_EXECUTED_FLAG) || sc.IsSet(ScriptContext::CONTINUE_EXECUTED_FLAG)))
                break;
        }

        sc.PopLocalScope();
    }

    void InterpreterStatementBlock::AddStatement(std::shared_ptr<InterpreterStatement> statement) {
        statements.push_back(statement);
    }

}

