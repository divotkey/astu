/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <stdexcept>

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

// Local includes
#include "Events.h"
#include "SdlVideoService.h"

namespace astu {

    SdlVideoService::SdlVideoService()
        : Service("SDL Video Service")
        , window(nullptr)
        , winWidth(1366)
        , winHeight(768)
        , winTitle("ASTU Window")
        , vulkanSupport(false)
        , resizeable(false)
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

    void SdlVideoService::SetResizeable(bool b)
    {
        if (GetStatus() != Stopped) {
            throw std::logic_error("Resizeable window cannot be enabled/disabled while video service is running.");
        }
        resizeable = b;
    }

    bool SdlVideoService::IsResizeable() const
    {
        return resizeable;
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

        auto resizeSrv = ASTU_GET_SERVICE_OR_NULL(ResizeEventService);
        if (resizeSrv) {
            resizeSrv->QueueSignal(ResizeEvent(winWidth, winHeight));
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

    SDL_Window* SdlVideoService::GetSdlWindow()
    {
        return window;
    }

} // end of namespace