// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "DeepSleep.h"

namespace astu {

    XosSleep::XosSleep() {
        // Intentionally left empty.
    }

    XosSleep::~XosSleep() {
        // Intentionally left empty.
    }

    void XosSleep::Sleep(std::chrono::nanoseconds ns) {
        struct timespec ts;
        ts.tv_sec = ns.count() / 1000000000;
        ts.tv_nsec = ns.count() % 1000000000;
        while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
    }

} // end of namespace
