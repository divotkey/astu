/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <iostream>
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
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL event service");

        if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL events subsystem: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        mouseButtonSrv = GetSM().FindService<MouseButtonEventService>();

        quit = false;
    }

    void SdlEventService::OnShutdown() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL event service");
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

            case SDL_MOUSEMOTION:
                mouse.SetCursor(event.motion.x, event.motion.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouse.SetButton(event.button.button, true);
                if(mouseButtonSrv) {
                    mouseButtonSrv->FireSignal(MouseButtonEvent(event.button.button, true));
                }
                break;

            case SDL_MOUSEBUTTONUP:
                mouse.SetButton(event.button.button, false);
                if(mouseButtonSrv) {
                    mouseButtonSrv->FireSignal(MouseButtonEvent(event.button.button, false));
                }
                break;

            case SDL_KEYDOWN:
                keyboard.SetKey(event.key.keysym.scancode, true);
                break;

            case SDL_KEYUP:
                keyboard.SetKey(event.key.keysym.scancode, false);
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