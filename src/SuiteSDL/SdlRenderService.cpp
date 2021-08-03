/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local incluees
#include "Service/ServiceManager.h"
#include "SuiteSDL/SdlVideoService.h"
#include "SuiteSDL/SdlRenderService.h"
#include "Graphics/WebColors.h"

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

// C++ Standard LIbrary includes
#include <cassert>
#include <algorithm>
#include <stdexcept>

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
        , vsync(true)
    {
        // Intentionally left empty.
    }

    void SdlRenderService::OnStartup() 
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, 
            "Starting up SDL render service");

        CreateRenderer();
        LogRendererInfo();

        // Fire resize event.
        auto & wm = ASTU_SERVICE(WindowService);
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

    void SdlRenderService::CreateRenderer()
    {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        uint32_t flags = SDL_RENDERER_ACCELERATED;
        if (vsync) {
            flags |= SDL_RENDERER_PRESENTVSYNC;
        }

        renderer = SDL_CreateRenderer(
            ASTU_SERVICE(SdlVideoService).GetSdlWindow(),
            -1, 
            flags 
            );

        if (!renderer) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, 
                "Couldn't create SDL render: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }

        if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Couldn't set blend mode for SDL Renderer: %s", SDL_GetError());        
            auto && lastError = SDL_GetError();
            SDL_DestroyRenderer(renderer);
            throw std::runtime_error(lastError);
        }            
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
            auto & wm = ASTU_SERVICE(WindowService);
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

    void SdlRenderService::SetBackgroundColor(const Color4f & bg)
    {
        backgroundColor = bg;
    }

    const Color4f& SdlRenderService::GetBackgroundColor() const {
        return backgroundColor;
    }

    size_t SdlRenderService::NumRenderLayers() const {
        return layers.size();
    }

    void SdlRenderService::SetVSync(bool b)
    {
        if (vsync == b) {
            return;
        }

        vsync = b;
        if (renderer) {
            CreateRenderer();
        }
    }

    bool SdlRenderService::IsVsync() const 
    {
        return vsync;
    }

    /////////////////////////////////////////////////
    /////// SdlRenderLayer
    /////////////////////////////////////////////////

    SdlRenderLayer::SdlRenderLayer(int renderPriority)
        : renderPriority(renderPriority)
    {
        AddStartupHook([this]() { ASTU_SERVICE( SdlRenderService).AddLayer(*this); });
        AddShutdownHook([this]() { ASTU_SERVICE( SdlRenderService).RemoveLayer(*this); });
    }

    void SdlRenderLayer::OnResize(int width, int height)
    {
        targetWidth = width;
        targetHeight = height;        
    }

    int SdlRenderLayer::GetRenderPriority() const
    {
        return renderPriority;
    }

} // end of namespace
