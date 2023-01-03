/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

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



