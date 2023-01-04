/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "StdSleep.h"

// C++ Standard Library includes
#include <thread>

using namespace std::this_thread;

using namespace std::chrono;

namespace astu {

    void StdSleep::Sleep(nanoseconds ns) {
        sleep_for(ns);
    }

} // end of namespace
