// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "ItemStateFunction.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateFunction::Copy() const {
        return make_unique<ItemStateFunction>(function);
    }

    std::shared_ptr<Item>
    ItemStateFunction::CallAsFunction(ScriptContext &sc,
                                      InterpreterActualParameterList &parameters,
                                      unsigned int lineNumber)
    {
        return function->Evaluate(sc, parameters, lineNumber);
    }

}