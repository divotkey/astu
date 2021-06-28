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
        : Service("SDL Vertex Buffer 2D Builder Service")
    {
        Reset();
    }

    VertexBuffer2Builder& SdlVertexBuffer2BuilderService::AddVertex(float x, float y)
    {
        vertices.push_back({x, y});
        return *this;
    }

    Vector2f SdlVertexBuffer2BuilderService::GetVertex(size_t idx)
    {
        return vertices.at(idx);
    }

    size_t SdlVertexBuffer2BuilderService::GetNumVertices() const
    {
        return vertices.size();
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
    /////// SdlSceneGraph2
    /////////////////////////////////////////////////

    SdlSceneGraph2::SdlSceneGraph2(int renderPriority, int updatePriority)
        : Service("SDL Scene Graph 2D")
        , Updatable(updatePriority)
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
        sceneRenderer->SetViewMatrix( GetCamera().GetMatrix() );
        sceneRenderer->SetSdlRenderer( *renderer );
        GetRoot()->Render( *sceneRenderer );
        sceneRenderer->ClearSdlRenderer();
    }

    void SdlSceneGraph2::OnUpdate()
    {
        GetRoot()->Update( GetElapsedTime() );
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
