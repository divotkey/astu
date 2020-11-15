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

    SdlEventService::SdlEventService(int priority)
        : UpdatableBaseService("SDL Event", priority)
    {
        // Intentioniall left empty.
    }

    void SdlEventService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL Event service");

        if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL events subsystem: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        quit = false;
    }

    void SdlEventService::OnShutdown() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL Event service");
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    }

    void SdlEventService::OnUpdate()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_DROPTEXT:
                // std::cout << "drop text" << std::endl;
                break;

            case SDL_DROPBEGIN:
                // std::cout << "drop begin" << std::endl;
                break;

            case SDL_DROPCOMPLETE:
                // std::cout << "drop complete" << std::endl;
                break;

            case SDL_DROPFILE:
                // std::cout << "file dropped: '" << event.drop.file << "'" << std::endl;
                SDL_free(event.drop.file);
                break;
            }
        }
    }

    bool SdlEventService::IsQuit() const 
    {
        return quit;
    }

    void SdlEventService::ClearQuit() 
    {
        quit = false;
    }


} // end of namespace