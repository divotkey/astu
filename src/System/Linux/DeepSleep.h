/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Util/IThreadSleep.h"

// Linux API includes
#define WIN32_LEAN_AND_MEAN
#include <time.h>
#include <errno.h>

namespace astu {

    /**
     * Implementation of the IThreadSleep interface using Linux API.
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
    };

} // end of namespace

