/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SpinLockSleep.h"

using namespace std::chrono;

namespace astu {

    void SpinLockSleep::Sleep(nanoseconds ns) {
        auto targetTime = high_resolution_clock ::now() + ns;
        while (targetTime > high_resolution_clock::now());
    }

} // end of namespace
