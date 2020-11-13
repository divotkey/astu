/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <SDL2/SDL.h>
#include "ServiceManager.h"
#include "SdlVideoService.h"
#include "SdlRenderService.h"

namespace astu {

    SdlRenderService::SdlRenderService()
        : renderer(nullptr)
    {
        // Intentionally left empty.
    }

    void SdlRenderService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL Renderer service");

        renderer = SDL_CreateRenderer(
            GetSM().GetService<SdlVideoService>().GetSdlWindow(), 
            -1, 
            SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC
            );

        if (!renderer) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't create SDL Renderer: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        LogRendererInfo();
    }

    void SdlRenderService::OnShutdown()
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL Renderer service");
       if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
    }

    void SdlRenderService::SdlRenderService::OnUpdate()
    {
        SDL_RenderPresent(renderer);
    }

    void SdlRenderService::LogRendererInfo()
    {
        assert(renderer);

        SDL_RendererInfo info;
        if (SDL_GetRendererInfo(renderer, &info)) {
            SDL_LogError(
                SDL_LOG_CATEGORY_VIDEO, 
                "Couldn't query information for SDL renderer: %s", 
                SDL_GetError());        
            return;
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Using SDL renderer '%s'", info.name);
        SDL_LogDebug(
            SDL_LOG_CATEGORY_RENDER, 
            "Maximum texture size %d x %d", 
            info.max_texture_width, 
            info.max_texture_height);
    }

} // end of namespace
