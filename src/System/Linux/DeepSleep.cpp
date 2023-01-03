/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "DeepSleep.h"

namespace astu {

    DeepSleep::DeepSleep() {
        // Intentionally left empty.
    }

    DeepSleep::~DeepSleep() {
        // Intentionally left empty.
    }

    void DeepSleep::Sleep(std::chrono::nanoseconds ns) {
        struct timespec ts;
        ts.tv_sec = ns.count() / 1000000000;
        ts.tv_nsec = ns.count() % 1000000000;
        while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
    }

} // end of namespace
