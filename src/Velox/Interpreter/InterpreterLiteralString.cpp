// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/InterpreterLiteralString.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateString.h"

using namespace std;

namespace velox {

    shared_ptr<Item> InterpreterLiteralString::Evaluate(ScriptContext &sc) {
        return Item::CreateString(value);
    }

}
