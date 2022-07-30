// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
    class XosSleep : public IThreadSleep {
    public:

        /**
         * Constructor.
         */
        XosSleep();

        /**
         * Destructor.
         */
        virtual ~XosSleep();

        // Inherited via IThreadSleep
        virtual void Sleep(std::chrono::nanoseconds ns) override;
    };

} // end of namespace

