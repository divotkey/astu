/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <SDL2/SDL.h>
#include "SdlService.h"

namespace astu {

    SdlService::SdlService(bool debug, bool verbose)
        : BaseService("SDL")
        , debugMode(debug)
        , verboseLogging(verbose)
    {
        // Intentioniall left empty.
    }

    void SdlService::OnStartup() 
    {
        if (debugMode) {
            if (verboseLogging) {
                SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
            } else {
                SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
            }
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL core service");

        if (SDL_Init(0) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }
    }

    void SdlService::OnShutdown() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL core service");
        SDL_Quit();
    }

} // end of namespace