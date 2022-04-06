// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Interpreter.h"
#include "Item.h"
#include "ItemStateFunction.h"

using namespace std;

namespace velox {

    Interpreter::Interpreter() : superGlobals(make_shared<Scope>()) {
        // Intentionally left empty.
    }

    void velox::Interpreter::Execute(std::shared_ptr<velox::InterpreterStatement> program) {
        context.ClearFlags();
        program->Execute(context);
    }

    void Interpreter::AddFunction(const std::string &name, std::shared_ptr<InterpreterFunction> function) {
        auto item = Item::Create(make_unique<ItemStateFunction>(function));
        superGlobals->AddItem(name, item);
    }

    void Interpreter::ClearVariables() {
        context.Clear();
        context.PushScope(superGlobals);
    }

    void Interpreter::ClearAll() {
        superGlobals->Clear();
        ClearVariables();
    }

}
