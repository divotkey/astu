// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// C++ Standard Library includes
#include <chrono>

namespace astu {

    /**
     * Interface for XOS specific thread suspension.
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