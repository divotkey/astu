/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#include "Service/ConsoleTimeService.h"

using namespace std;

namespace astu {

    ConsoleTimeService::ConsoleTimeService(int priority)
        : Service("Console Time Service")
        , Updatable(priority)
    {
        // Intentionally left empty.
    }

    void ConsoleTimeService::OnStartup() {
        timeInNs = chrono::nanoseconds(0);
        prevTime = chrono::high_resolution_clock::now();
    }

    void ConsoleTimeService::OnShutdown() {
        // Intentionally left empty.
    }

    void ConsoleTimeService::OnUpdate() {
        auto now = chrono::high_resolution_clock::now();
        auto elapsed = now - prevTime;
        prevTime = now;
        timeInNs += chrono::duration_cast<chrono::nanoseconds>(elapsed);
        deltaTime = chrono::duration<double>(elapsed).count();
        //deltaTime = elapsed.count() / 1e9;
    }

    double ConsoleTimeService::GetAbsoluteTime() const {
        return static_cast<double>(timeInNs.count() / 1000000000.0);
    }

    double ConsoleTimeService::GetElapsedTime() const {
        return deltaTime;
    }

} // end of namespace