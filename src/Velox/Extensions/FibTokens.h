/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Util/Memento.h"

namespace astu {

    class FibTokens {
    public:
        static const unsigned char data[];
        static const size_t size;
        static Memento GetMemento();
    };

} // end of namespace
