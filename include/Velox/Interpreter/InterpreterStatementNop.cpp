// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "InterpreterStatementNop.h"

namespace velox {

    void InterpreterStatementNop::Execute(ScriptContext &sc) {
        return;
    }

}