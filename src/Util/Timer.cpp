/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

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
        startTime = chrono::high_resolution_clock::now();
    }

    void Timer::Stop() {
        if (!running) return;
        elapsed = chrono::high_resolution_clock::now() - startTime;
        running = false;
    }

    unsigned int Timer::GetMilliseconds() const {
        if (running) {
            auto delta = chrono::high_resolution_clock::now() - startTime;
            return static_cast<unsigned int>(chrono::duration_cast<chrono::milliseconds>(elapsed + delta).count());
        }

        return static_cast<unsigned int>(chrono::duration_cast<chrono::milliseconds>(elapsed).count());
    }

    unsigned int Timer::GetMicroseconds() const {
        if (running) {
            auto delta = chrono::high_resolution_clock::now() - startTime;
            return static_cast<unsigned int>(chrono::duration_cast<chrono::microseconds>(elapsed + delta).count());
        }

        return static_cast<unsigned int>(chrono::duration_cast<chrono::microseconds>(elapsed).count());
    }

};