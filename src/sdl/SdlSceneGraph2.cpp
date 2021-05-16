/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <cassert>     
#include <SDL2/SDL.h>

// Local includes
#include "SdlScene2Renderer.h"
#include "SdlSceneGraph2.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// SdlVertexBuffer2Builder
    /////////////////////////////////////////////////

    SdlVertexBuffer2BuilderService::SdlVertexBuffer2BuilderService()
    {
        Reset();
    }

    VertexBuffer2Builder& SdlVertexBuffer2BuilderService::AddVertex(float x, float y)
    {
        vertices.push_back({x, y});
        return *this;
    }

    VertexBuffer2Builder& SdlVertexBuffer2BuilderService::Reset()
    {
        vertices.clear();
        return *this;
    }

    std::shared_ptr<VertexBuffer2> SdlVertexBuffer2BuilderService::Build()
    {
        auto result = std::make_shared<SdlVertexBuffer2>();
        result->vertices = vertices;
        return result;
    }

    /////////////////////////////////////////////////
    /////// SdlVertexBuffer2Builder
    /////////////////////////////////////////////////

    SdlSceneGraph2::SdlSceneGraph2(int renderPriority)
        : Service("SDL Scene Graph (2D)")
        , SdlRenderLayer(renderPriority)
    {
        // Intentionally left empty.
    }
    
    SdlSceneGraph2::~SdlSceneGraph2()
    {
        // Intentionally left empty.
    }

    void SdlSceneGraph2::OnRender(SDL_Renderer* renderer)
    {
        GetRoot().Update();
        sceneRenderer->SetSdlRenderer(*renderer);
        GetRoot().Render(*sceneRenderer);
        sceneRenderer->ClearSdlRenderer();
    }

    void SdlSceneGraph2::OnStartup()
    {
        sceneRenderer = std::make_unique<SdlScene2Renderer>();
    }

    void SdlSceneGraph2::OnShutdown()
    {
        sceneRenderer = nullptr;
    }

} // end of namespace
