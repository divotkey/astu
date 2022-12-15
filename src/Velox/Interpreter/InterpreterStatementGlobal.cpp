// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterStatementGlobal.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/Item.h"

namespace velox {

    void InterpreterStatementGlobal::Execute(ScriptContext &sc) {
        if (sc.FindLocalItem(name)) {
            throw InterpreterError("variable '" + name + "' is already declared within local scope", GetLineNumber());
        }

        auto item = sc.FindGlobalItem(name);
        if (!item) {
            item = Item::CreateUndefined();
            sc.AddGlobalItem(name, item);
        }

        if (sc.FindLocalItem(name)) {
            // It seems that there is no local scope and hence we are done.
             return;
        }

        sc.AddGlobalItem(name, item);
    }

    void InterpreterStatementGlobal::SetName(const std::string &inName) {
        name = inName;
    }
}