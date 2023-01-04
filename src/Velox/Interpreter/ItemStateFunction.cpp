/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/ItemStateFunction.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateFunction::Copy() const {
        return make_unique<ItemStateFunction>(function);
    }

    std::shared_ptr<Item>
    ItemStateFunction::CallAsFunction(ScriptContext &sc,
                                      InterpreterActualParameterList &parameters,
                                      std::shared_ptr<Scope> memberScope,
                                      unsigned int lineNumber)
    {
        // Create block scope to have a home for temporary items.
        sc.PushCodeBlockScope();
        auto result = function->Evaluate(sc, parameters, memberScope, lineNumber);
        sc.PopLocalScope();
        return result;
    }

    ItemType ItemStateFunction::GetType() const {
        return ItemType::Other;
    }

}