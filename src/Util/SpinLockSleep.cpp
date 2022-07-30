// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "SpinLockSleep.h"

using namespace std::chrono;

namespace astu {

    void SpinLockSleep::Sleep(nanoseconds ns) {
        auto targetTime = high_resolution_clock ::now() + ns;
        while (targetTime > high_resolution_clock::now());
    }

} // end of namespace
