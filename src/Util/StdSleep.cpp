// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
