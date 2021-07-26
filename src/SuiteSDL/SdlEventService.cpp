/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "SuiteSDL/SdlEventService.h"
#include "SuiteSDL/ISdlResizeListener.h"
#include "ISdlEventListener.h"
#include "SdlKeyTable.h"

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

// C++ Standard library includes
#include <stdexcept>
#include <algorithm>
#include <iostream>

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

        inputMapperSrv = ASTU_GET_SERVICE_OR_NULL(InputMappingService);
        mouseButtonSrv = ASTU_GET_SERVICE_OR_NULL(MouseButtonSignalService);
        mouseWheelSrv = ASTU_GET_SERVICE_OR_NULL(MouseWheelSignalService);
        mouseMoveSrv = ASTU_GET_SERVICE_OR_NULL(MouseMoveSignalService);
        keystrokeSrv = ASTU_GET_SERVICE_OR_NULL(KeystrokeSignalService);
        resizeSrv = ASTU_GET_SERVICE_OR_NULL(ResizeSignalService);
        windowStateSrv = ASTU_GET_SERVICE_OR_NULL(WindowStateSignalService);
        
        quit = false;
    }

    void SdlEventService::OnShutdown() 
    {
        windowStateSrv = nullptr;
        resizeSrv = nullptr;
        keystrokeSrv = nullptr;
        mouseWheelSrv = nullptr;
        mouseButtonSrv = nullptr;
        mouseMoveSrv = nullptr;
        inputMapperSrv = nullptr;
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, 
            "Shutting down SDL event service");
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    }

    void SdlEventService::OnUpdate()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            // Uncomment for debugging.
            // std::cout << "event: " << event.type << std::endl;

            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                if (windowStateSrv) {
                    windowStateSrv->QueueSignal(
                        WindowState(WindowState::Type::CLOSE));
                }
                break;

            case SDL_MOUSEMOTION:
                mouse.SetCursor(event.motion.x, event.motion.y);
                if (mouseMoveSrv) {
                    mouseMoveSrv->FireSignal(
                        MouseMoveSignal(event.motion.x, event.motion.y));
                }
                break;

            case SDL_MOUSEWHEEL:
                if (event.wheel.y != 0 && mouseWheelSrv) {
                    // std::cout << "mouse wheel vertical movement: " << event.wheel.y << std::endl;
                    mouseWheelSrv->FireSignal(MouseWheelSignal(event.wheel.y));
                }
                if (event.wheel.x != 0) {
                    // std::cout << "mouse wheel horizontal movement" << std::endl;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouse.SetButton(event.button.button, true);
                if(mouseButtonSrv) {
                    mouseButtonSrv->FireSignal(
                            MouseButtonSignal(
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
                        MouseButtonSignal(
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
                        KeystrokeSignal(event.key.keysym.scancode, true) );
                }
                if (inputMapperSrv) {
                    inputMapperSrv->ProcessKey( 
                        SdlKeyTable::ScanCodeToKey(event.key.keysym.scancode), 
                        true
                    );
                }
                break;

            case SDL_KEYUP:
                keyboard.SetKey(event.key.keysym.scancode, false);
                if (keystrokeSrv) {
                    keystrokeSrv->FireSignal( 
                            KeystrokeSignal(event.key.keysym.scancode, false) );
                }
                if (inputMapperSrv) {
                    inputMapperSrv->ProcessKey( 
                        SdlKeyTable::ScanCodeToKey(event.key.keysym.scancode), 
                        false
                    );
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
                                ResizeSignal(
                                    event.window.data1, 
                                    event.window.data2)
                                );
                    }

                    // Propagate event to registered listeners.
                    for (auto listener : resizeListeners) {
                        listener->OnResize(event.window.data1, event.window.data2);
                    }
                    break;

                case SDL_WINDOWEVENT_MINIMIZED:
                    if (windowStateSrv) {
                        windowStateSrv->QueueSignal(
                            WindowState(WindowState::Type::MINIMIZED));
                    }
                    break;

                case SDL_WINDOWEVENT_MAXIMIZED:
                    if (windowStateSrv) {
                        windowStateSrv->QueueSignal(
                            WindowState(WindowState::Type::MAXIMIZED));
                    }
                    break;

                case SDL_WINDOWEVENT_SHOWN:
                    if (windowStateSrv) {
                        windowStateSrv->QueueSignal(
                            WindowState(WindowState::Type::SHOWN));
                    }
                    break;
                }
                
                break;

            case SDL_TEXTINPUT:
                // ignore.
                break;

            case SDL_TEXTEDITING:
                // ignore.
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

            default:
                // Propagate event to registered listeners.
                for (auto listener : eventListeners) {
                    listener->HandleEvent(event);
                }
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

    bool SdlEventService::HasSdlEventListener(ISdlEventListener & listener) const
    {
        return find(eventListeners.begin(), eventListeners.end(), &listener) != eventListeners.end();
    }

    void SdlEventService::AddSdlEventListener(ISdlEventListener & listener)
    {
        if (HasSdlEventListener(listener)) {
            throw std::logic_error("SDL event listener already added");
        }

        eventListeners.push_back(&listener);
    }

    void SdlEventService::RemoveSdlEventListener(ISdlEventListener & listener)
    {
        eventListeners.erase(
            remove(eventListeners.begin(), eventListeners.end(), &listener),
            eventListeners.end());
    }

    bool SdlEventService::HasSdlResizeListener(ISdlResizeListener & listener) const
    {
        return find(resizeListeners.begin(), resizeListeners.end(), &listener) != resizeListeners.end();
    }

    void SdlEventService::AddSdlResizeListener(ISdlResizeListener & listener)
    {
        if (HasSdlResizeListener(listener)) {
            throw std::logic_error("SDL resize listener already added");
        }

        resizeListeners.push_back(&listener);
    }

    void SdlEventService::RemoveSdlResizeListener(ISdlResizeListener & listener)
    {
        resizeListeners.erase(
            remove(resizeListeners.begin(), resizeListeners.end(), &listener),
            resizeListeners.end());
    }


} // end of namespace