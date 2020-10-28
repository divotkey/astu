/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <SDL2/SDL.h>
#include "SdlEventService.h"

namespace astu {

    SdlEventService::SdlEventService()
        : BaseService("SDL Event")
    {
        // Intentioniall left empty.
    }

    void SdlEventService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL event service");

        if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL events subsystem: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }
    }

    void SdlEventService::OnShutdown() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL event service");
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    }

} // end of namespace