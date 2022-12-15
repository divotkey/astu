// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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