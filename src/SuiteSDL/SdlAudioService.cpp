/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "SuiteSDL/SdlAudioService.h"

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

// C++ Standard Library includes
#include <stdexcept>

namespace astu {

    SdlAudioService::SdlAudioService()
        : Service("SDL Audio Service")
    {
        // Intentionally left empty.
    }

    void SdlAudioService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL audio service");

        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL audio subsystem: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        LogAudioInfo();
    }

    void SdlAudioService::CleanUp()
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    void SdlAudioService::OnShutdown()
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL audio service");
        CleanUp();
    }

    void SdlAudioService::LogAudioInfo()
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_AUDIO, "Using audio driver: %s", SDL_GetCurrentAudioDriver());        

        SDL_LogDebug(SDL_LOG_CATEGORY_AUDIO, "Found %d playback devices:", SDL_GetNumAudioDevices(0));        
        for (int i = 0; i < SDL_GetNumAudioDevices(0); ++i) {
            SDL_LogDebug(SDL_LOG_CATEGORY_AUDIO, "devices #%d: %s", i, SDL_GetAudioDeviceName(i, 0));        
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_AUDIO, "Found %d capturing devices:", SDL_GetNumAudioDevices(1));        
        for (int i = 0; i < SDL_GetNumAudioDevices(1); ++i) {
            SDL_LogDebug(SDL_LOG_CATEGORY_AUDIO, "devices #%d: %s", i, SDL_GetAudioDeviceName(i, 1));        
        }
    }



} // end of namespace