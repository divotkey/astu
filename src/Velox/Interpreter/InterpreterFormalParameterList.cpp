// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/InterpreterFormalParameterList.h"

#include <cassert>

namespace velox {

    size_t InterpreterFormalParameterList::NumParameters() const {
        return parameters.size();
    }

    const std::string &InterpreterFormalParameterList::GetParameter(size_t idx) const {
        assert(idx < parameters.size());
        return parameters[idx];
    }

}
