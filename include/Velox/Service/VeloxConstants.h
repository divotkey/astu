/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>

namespace astu {

    /**
     * This class functions primarily as namespace for Velox related constants.
     */
    class VeloxConstants {
    public:
        /** The logging tag used to identify logging messages. */
        static const std::string LOGGING_TAG;

        /**
         * No instances allowed.
         */
        VeloxConstants() = delete;

        /**
         * No instances allowed.
         */
        VeloxConstants( const VeloxConstants& ) = delete;

        /**
         * No copies allowed.
         *
         * @return reference to this class.
         */
        VeloxConstants& operator=( const VeloxConstants& ) = delete;
    };

} // end of namespace