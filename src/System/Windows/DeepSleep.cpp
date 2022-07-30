// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "DeepSleep.h"

// C++ Standard Library includes
#include <stdexcept>

namespace astu {

    DeepSleep::DeepSleep() {
        if(!(hTimer = CreateWaitableTimer(NULL, TRUE, NULL)))  {
            throw std::runtime_error("Unable to create windows hTimer");
        }
    }

    DeepSleep::~DeepSleep() {
        if (hTimer)
            CloseHandle(hTimer);
    }

    void DeepSleep::Sleep(std::chrono::nanoseconds ns) {
        LARGE_INTEGER li;
        li.QuadPart = -ns.count() / 100;
        if (!SetWaitableTimer(hTimer, &li, 0, NULL, NULL, FALSE)) {
            throw std::runtime_error("Unable to set windows timer");
        }

        WaitForSingleObject(hTimer, INFINITE);
    }

} // end of namespace
