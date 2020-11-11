/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <SDL2/SDL.h>
#include "SdlVideoService.h"

namespace astu {

    SdlVideoService::SdlVideoService()
        : BaseService("SDL Video")
        , window(nullptr)
        , winWidth(1366)
        , winHeight(768)
        , winTitle("ASTU Window")
    {
        // Intentionally left empty.
    }

    void SdlVideoService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL Video service");

        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL Video subsystem: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        window = SDL_CreateWindow(
            winTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED,
            winWidth,
            winHeight,
            SDL_WINDOW_SHOWN
        );

        if (!window) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't create SDL Window: %s", SDL_GetError());        
            CleanUp();
            throw std::runtime_error(SDL_GetError());
        }
    }

    void SdlVideoService::CleanUp()
    {
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }


    void SdlVideoService::OnShutdown()
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL Video service");
        CleanUp();
    }

    void SdlVideoService::SetSize(int width, int height) 
    {
        if (IsRunning()) {
            throw std::logic_error("Windows size must not be changed during runtime.");
        }
        winWidth = width;
        winHeight = height;
    }
    
    int SdlVideoService::GetWidth() const 
    {
        return winWidth;
    }

    int SdlVideoService::GetHeight() const 
    {
        return winHeight;
    }

    void SdlVideoService::SetTitle(const std::string & title) 
    {
        winTitle = title;
        if (IsRunning()) {
            SDL_SetWindowTitle(window, title.c_str());
        }
    }

    const std::string & SdlVideoService::GetTitle() const 
    {
        return winTitle;
    }

} // end of namespace