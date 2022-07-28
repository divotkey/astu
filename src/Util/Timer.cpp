// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Util/Timer.h"

using namespace std;

namespace astu {

    Timer::Timer() : running(false) {
        // intentionally left empty.
    }

    void Timer::Start() {
        if (running) return;
        running = true;
        elapsed = std::chrono::milliseconds::zero();
        startTime = chrono::steady_clock::now();
    }

    void Timer::Stop() {
        if (!running) return;
        elapsed = chrono::steady_clock::now() - startTime;
        running = false;
    }

    void Timer::Resume() {
        if (!running) return;
        running = true;
        startTime = chrono::steady_clock::now();
    }

    unsigned int Timer::GetMilliseconds() const {
        if (running) {
            auto delta = chrono::steady_clock::now() - startTime;
            return static_cast<unsigned int>(chrono::duration_cast<chrono::milliseconds>(elapsed + delta).count());
        }

        return static_cast<unsigned int>(chrono::duration_cast<chrono::milliseconds>(elapsed).count());
    }

    unsigned int Timer::GetMicroseconds() const {
        if (running) {
            auto delta = chrono::steady_clock::now() - startTime;
            return static_cast<unsigned int>(chrono::duration_cast<chrono::microseconds>(elapsed + delta).count());
        }

        return static_cast<unsigned int>(chrono::duration_cast<chrono::microseconds>(elapsed).count());
    }

};