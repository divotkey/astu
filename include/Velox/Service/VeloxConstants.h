/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>

namespace astu {

    class VeloxConstants {
    public:
        static const std::string LOGGING_TAG;

        VeloxConstants() = delete;
        VeloxConstants( const VeloxConstants& ) = delete;
        VeloxConstants& operator=( const VeloxConstants& ) = delete;
    };

} // end of namespace