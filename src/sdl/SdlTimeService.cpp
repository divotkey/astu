/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <SDL2/SDL.h>
#include "SdlTimeService.h"

namespace astu {

    SdlTimeService::SdlTimeService(int priority)
        : UpdatableBaseService("SDL Time", priority)
    {
        // Intentioniall left empty.
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
        // Intentioniall left empty.
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


} // end of namespace