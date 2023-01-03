/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

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
