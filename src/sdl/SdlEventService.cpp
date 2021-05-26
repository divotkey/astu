/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <iostream>
#include <SDL2/SDL.h>
#include "SdlEventService.h"

namespace astu {

    SdlEventService::SdlEventService(int priority)
        : Service("SDL Event Service")
        , Updatable(priority)
    {
        // Intentionally left empty.
    }

    void SdlEventService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, 
            "Starting up SDL event service");

        if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                "Couldn't initialize SDL events subsystem: %s", SDL_GetError());     
                  
            throw std::runtime_error(SDL_GetError());
        }

        mouseButtonSrv = ASTU_GET_SERVICE_OR_NULL(MouseButtonEventService);
        mouseWheelSrv = ASTU_GET_SERVICE_OR_NULL(MouseWheelEventService);
        mouseMoveSrv = ASTU_GET_SERVICE_OR_NULL(MouseMoveEventService);
        keystrokeSrv = ASTU_GET_SERVICE_OR_NULL(KeystrokeEventService);
        resizeSrv = ASTU_GET_SERVICE_OR_NULL(ResizeEventService);
        quit = false;
    }

    void SdlEventService::OnShutdown() 
    {
        resizeSrv = nullptr;
        keystrokeSrv = nullptr;
        mouseWheelSrv = nullptr;
        mouseButtonSrv = nullptr;
        mouseMoveSrv = nullptr;
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, 
            "Shutting down SDL event service");
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
                if (mouseMoveSrv) {
                    mouseMoveSrv->FireSignal(
                        MouseMoveEvent(event.motion.x, event.motion.y));
                }
                break;

            case SDL_MOUSEWHEEL:
                if (event.wheel.y != 0 && mouseWheelSrv) {
                    // std::cout << "mouse wheel vertical movement: " << event.wheel.y << std::endl;
                    mouseWheelSrv->FireSignal(MouseWheelEvent(event.wheel.y));
                }
                if (event.wheel.x != 0) {
                    // std::cout << "mouse wheel horizontal movement" << std::endl;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouse.SetButton(event.button.button, true);
                if(mouseButtonSrv) {
                    mouseButtonSrv->FireSignal(
                            MouseButtonEvent(
                                event.button.button, 
                                true, 
                                event.button.x, 
                                event.button.y)
                                );
                }
                break;

            case SDL_MOUSEBUTTONUP:
                mouse.SetButton(event.button.button, false);
                if(mouseButtonSrv) {
                    mouseButtonSrv->FireSignal( 
                        MouseButtonEvent(
                            event.button.button, 
                            false, 
                            event.button.x,
                            event.button.y)
                        );
            }
                break;

            case SDL_KEYDOWN:
                keyboard.SetKey(event.key.keysym.scancode, true);
                // std::cout << "scancode: " << event.key.keysym.scancode << std::endl;
                if (keystrokeSrv) {
                    keystrokeSrv->FireSignal( 
                        KeystrokeEvent(event.key.keysym.scancode, true) );
                }
                break;

            case SDL_KEYUP:
                keyboard.SetKey(event.key.keysym.scancode, false);
                if (keystrokeSrv) {
                    keystrokeSrv->FireSignal( 
                            KeystrokeEvent(event.key.keysym.scancode, false) );
                }
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    // Ignore.
                    break;

                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    if (resizeSrv) {
                        resizeSrv->FireSignal( 
                                ResizeEvent(
                                    event.window.data1, 
                                    event.window.data2)
                                );
                    }
                    break;
                }
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