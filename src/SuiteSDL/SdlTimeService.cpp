/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SuiteSDL/SdlTimeService.h"

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

namespace astu {

    SdlTimeService::SdlTimeService(int priority)
        : Service("SDL Time Service")
        , Updatable(priority)
    {
        // Intentionally left empty.
    }

    void SdlTimeService::OnStartup() 
    {
        performToSeconds = 1.0 / SDL_GetPerformanceFrequency();
        performCnt = SDL_GetPerformanceCounter();
        time = 0;
        deltaTime = 0;
    }

    void SdlTimeService::OnShutdown() 
    {
        // Intentionally left empty.
    }

    void SdlTimeService::OnUpdate()
    {
        uint64_t now = SDL_GetPerformanceCounter();
        deltaTime = (now - performCnt) * performToSeconds;
        performCnt = now;
        time += deltaTime;
    }

    double SdlTimeService::GetElapsedTime() const
    {
        return deltaTime;
    }

    double SdlTimeService::GetAbsoluteTime() const
    {
        return time;
    }

} // end of namespace