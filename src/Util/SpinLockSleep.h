// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Util/IThreadSleep.h"

namespace astu {

    /**
     * Implements the IThreadSleep interface using a busy waiting strategy.
     * This implementation does not actually suspend the thread, but actively
     * waits until the requested amount of time has passed.
     */
    class SpinLockSleep : public IThreadSleep {
    public:

        // Inherited via IThreadSleep
        virtual void Sleep(std::chrono::nanoseconds ns) override;
    };

} // end of namespace



