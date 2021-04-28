/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard LIbrary includes
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <SDL2/SDL.h>

// Local incluees
#include "ServiceManager.h"
#include "SdlVideoService.h"
#include "SdlRenderService.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// SdlRenderService
    /////////////////////////////////////////////////

    bool compare(ISdlRenderLayer * l1, ISdlRenderLayer *l2) {
        return l1->GetRenderPriority() < l2->GetRenderPriority();
    }

    SdlRenderService::SdlRenderService(int priority)
        : Service("SDL Render Service")
        , Updatable(priority)
        , renderer(nullptr)
        , backgroundColor(WebColors::Black)
    {
        // Intentionally left empty.
    }

    void SdlRenderService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL render service");

        renderer = SDL_CreateRenderer(
            ASTU_SERVICE(SdlVideoService).GetSdlWindow(),
            -1, 
            SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC 
            );

        if (!renderer) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't create SDL render: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        LogRendererInfo();

        // Fire resize event.
        auto & wm = ASTU_SERVICE(IWindowManager);
        for (auto & layer : layers) {
            layer->OnResize(wm.GetWidth(), wm.GetHeight());
        }

    }

    void SdlRenderService::OnShutdown()
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL render service");
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
    }

    void SdlRenderService::SdlRenderService::OnUpdate()
    {
        SDL_SetRenderDrawColor(
            renderer, 
            static_cast<uint8_t>(backgroundColor.r * 255), 
            static_cast<uint8_t>(backgroundColor.g * 255), 
            static_cast<uint8_t>(backgroundColor.b * 255), 
            SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        for (auto & layer : layers) {
            layer->OnRender(renderer);
        }
        
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

    void SdlRenderService::AddLayer(ISdlRenderLayer &layer)
    {
        if (HasLayer(layer)) {
            throw std::logic_error("Render layer already added");
        }

        layers.push_back(&layer);
        std::sort(layers.begin(), layers.end(), compare);

        if (GetStatus() != Stopped) {
            auto & wm = ASTU_SERVICE(IWindowManager);
            layer.OnResize(wm.GetWidth(), wm.GetHeight());
        }
    }

    void SdlRenderService::RemoveLayer(ISdlRenderLayer &layer)
    {
        layers.erase(
            std::remove(layers.begin(), layers.end(), &layer), 
            layers.end()
        );        
    }

    bool SdlRenderService::HasLayer(ISdlRenderLayer &layer)
    {
        return std::find(layers.begin(), layers.end(), &layer) != layers.end();
    }

    /////////////////////////////////////////////////
    /////// BaseSdlRenderLayer
    /////////////////////////////////////////////////

    BaseSdlRenderLayer::BaseSdlRenderLayer(int renderPriority)
        : renderPriority(renderPriority)
    {
        AddStartupHook([this]() { ASTU_SERVICE( SdlRenderService).AddLayer(*this); });
        AddShutdownHook([this]() { ASTU_SERVICE( SdlRenderService).RemoveLayer(*this); });
    }

    void BaseSdlRenderLayer::OnResize(int width, int height)
    {
        targetWidth = width;
        targetHeight = height;        
    }

    int BaseSdlRenderLayer::GetRenderPriority() const
    {
        return renderPriority;
    }

} // end of namespace
