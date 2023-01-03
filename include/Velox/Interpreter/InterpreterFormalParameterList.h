/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
 */

#pragma once

//#include "SimpleNameInterpreterNode.h"
#include <vector>

// C++ Standard Library include
#include <cstddef>
#include <string>

namespace velox {

    class InterpreterFormalParameterList {
    public:

        size_t NumParameters() const;
        const std::string &GetParameter(size_t idx) const;

    private:
        std::vector<std::string> parameters;
    };

}



