/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/InterpreterStatementLoop.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    InterpreterStatementLoop::InterpreterStatementLoop(unsigned int lineNumber) : InterpreterStatement(lineNumber) {
        // Intentionally left empty.
    }

    void InterpreterStatementLoop::Execute(ScriptContext &sc) {
        sc.PushCodeBlockScope();

        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG);
        while (!sc.IsFlagSet(ScriptContext::BREAK_EXECUTED_FLAG) && !sc.IsFlagSet(ScriptContext::RETURN_EXECUTED_FLAG))
        {
            sc.ClearFlag(ScriptContext::CONTINUE_EXECUTED_FLAG);
            loopBody->Execute(sc);
        }
        sc.ClearFlag(ScriptContext::BREAK_EXECUTED_FLAG | ScriptContext::CONTINUE_EXECUTED_FLAG);
        sc.PopLocalScope();
    }

    void InterpreterStatementLoop::SetStatement(std::shared_ptr<InterpreterStatement> statement) {
        loopBody = statement;
    }

}