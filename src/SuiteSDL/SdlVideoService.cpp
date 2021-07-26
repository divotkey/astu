/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "SuiteSDL/SdlVideoService.h"
#include "SuiteSDL/SdlEventService.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <stdexcept>

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

namespace astu {

    SdlVideoService::SdlVideoService()
        : Service("SDL Video Service")
        , window(nullptr)
        , winWidth(1366)
        , winHeight(768)
        , winTitle("ASTU Window")
        , vulkanSupport(false)
        , resizeable(false)
        , fullscreen(false)
    {
        // Intentionally left empty.
    }

    bool SdlVideoService::IsVulkanSupportEnabled() const
    {
        return vulkanSupport;
    }

    SdlVideoService& SdlVideoService::EnableVulkanSupport(bool b)
    {
        if (GetStatus() != Stopped) {
            throw std::logic_error("Vulkan support cannot be enabled/disabled while video service is running.");
        }

        vulkanSupport = b;
        return *this;
    }

    void SdlVideoService::SetResizable(bool b)
    {
        if (GetStatus() != Stopped) {
            throw std::logic_error("Resizability of window cannot be enabled/disabled while video service is running.");
        }
        resizeable = b;
    }

    bool SdlVideoService::IsResizable() const
    {
        return resizeable;
    }

    int SdlVideoService::NumDisplays() const 
    {
        return SDL_GetNumVideoDisplays();
    }

    void SdlVideoService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL video service");

        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL video subsystem: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        uint32_t flags = SDL_WINDOW_SHOWN;
        if (vulkanSupport) {
            flags |= SDL_WINDOW_VULKAN;
        }

        if (resizeable) {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        window = SDL_CreateWindow(
            winTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED,
            winWidth,
            winHeight,
            flags
        );

        if (!window) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't create SDL window: %s", SDL_GetError());        
            CleanUp();
            throw std::runtime_error(SDL_GetError());
        }

        auto resizeSrv = ASTU_GET_SERVICE_OR_NULL(ResizeSignalService);
        if (resizeSrv) {
            resizeSrv->QueueSignal(ResizeSignal(winWidth, winHeight));
        }

        if (fullscreen) {
            EnableFullscreen();
        }

        ASTU_SERVICE(SdlEventService).AddSdlResizeListener(*this);
    }

    void SdlVideoService::CleanUp()
    {
        ASTU_SERVICE(SdlEventService).RemoveSdlResizeListener(*this);
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void SdlVideoService::OnShutdown()
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL video service");
        CleanUp();
    }

    void SdlVideoService::SetSize(int width, int height) 
    {
        if (GetStatus() != Stopped) {
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
        if (GetStatus() != Stopped) {
            SDL_SetWindowTitle(window, title.c_str());
        }
    }

    const std::string & SdlVideoService::GetTitle() const 
    {
        return winTitle;
    }

    void SdlVideoService::SetFullscreen(bool enableFullscreen)
    {
        if (fullscreen == enableFullscreen) {
            return;
        }

        if (IsStarted()) {
            if (enableFullscreen) {
                EnableFullscreen();
            } else {
                DisableFullscreen();
            }
        } else {
            fullscreen = enableFullscreen;
        }
    }

    bool SdlVideoService::IsFullscreen() const
    {
        return fullscreen;        
    }

    SDL_Window* SdlVideoService::GetSdlWindow()
    {
        return window;
    }

    void SdlVideoService::EnableFullscreen()
    {
        assert(window);

        SDL_DisplayMode wantedMode;
        wantedMode.w = winWidth;
        wantedMode.h = winHeight;
        wantedMode.format = 0;
        wantedMode.refresh_rate = 0;
        wantedMode.driverdata = nullptr;

        SDL_DisplayMode closestMode;
        if (!SDL_GetClosestDisplayMode(0, &wantedMode, &closestMode)) {
            SDL_LogWarn(SDL_LOG_CATEGORY_VIDEO, 
                "Couldn't' find resolution for fullscreen mode: %s", 
                SDL_GetError());
            return;
        }

        if (closestMode.w == winWidth && closestMode.h == winHeight) {
            if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0) {
                SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                    "Couldn't engage full-screen window: %s", 
                SDL_GetError());

                return;
            }
            fullscreen = true;
        } else {
            // Switch window size.
            SDL_SetWindowSize(window, closestMode.w, closestMode.h);
            if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0) {
                SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                    "Couldn't engage full-screen window: %s", 
                SDL_GetError());

                SDL_SetWindowSize(window, winWidth, winHeight);
                return;
            }
            fullscreen = true;
        }
    }

    void SdlVideoService::DisableFullscreen() {
        if (SDL_SetWindowFullscreen(window, 0) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Couldn't disable full-screen window: %s", 
            SDL_GetError());
            return;
        }
        
        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        if (w != winWidth || h != winHeight) {
            SDL_SetWindowSize(window, winWidth, winHeight);
        }

        fullscreen = false;
    }

    void SdlVideoService::OnResize(int width, int height)
    {
        winWidth = width;
        winHeight = height;
    }


} // end of namespace