// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Util/IThreadSleep.h"

// Windows API includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace astu {

    /**
     * Implementation of the IThreadSleep interface using Windows API.
     *
     * Note: this implementation is quite inaccurate on current Windows systems.
     * TODO use multimedia timers or combine spin lock with sleep.
     */
    class DeepSleep : public IThreadSleep {
    public:

        /**
         * Constructor.
         */
        DeepSleep();

        /**
         * Destructor.
         */
        virtual ~DeepSleep();

        // Inherited via IThreadSleep
        virtual void Sleep(std::chrono::nanoseconds ns) override;

    private:
        /** The hTimer handle. */
        HANDLE hTimer;
    };

} // end of namespace

