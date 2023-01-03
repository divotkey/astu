/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <chrono>

namespace astu {

    /**
     * Interface for OS specific thread suspension.
     */
    class IThreadSleep {
    public:
        /**
         * Virtual destructor.
         */
        virtual ~IThreadSleep() {}

        /**
         * Suspends the current thread for the specified amount of nanoseconds.
         *
         * @param ns    the duration in nanoseconds
         */
        virtual void Sleep(std::chrono::nanoseconds ns) = 0;
    };

} // end of namespace