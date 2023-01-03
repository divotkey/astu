/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
 */

#include "AstUtils0.h"
#include "JackErrorUtil.h"

namespace astu {

    int SetJackError(const std::string & s) {
        SetLastError(JACK_ERROR);
        SetErrorDetails(s.c_str());            
        return GetLastError();
    }

} // end of namespace