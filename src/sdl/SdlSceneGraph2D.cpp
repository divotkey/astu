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
#include "SdlSceneRenderer2D.h"
#include "SdlSceneGraph2D.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// SdlVertexBuffer2Builder
    /////////////////////////////////////////////////

    SdlVertexBufferBuilderService2D::SdlVertexBufferBuilderService2D()
        : Service("SDL Vertex Buffer 2D Builder Service")
    {
        Reset();
    }

    VertexBufferBuilder2D& SdlVertexBufferBuilderService2D::AddVertex(float x, float y)
    {
        vertices.push_back({x, y});
        return *this;
    }

    const Vector2f& SdlVertexBufferBuilderService2D::GetVertex(size_t idx) const
    {
        return vertices.at(idx);
    }

    void SdlVertexBufferBuilderService2D::SetVertex(size_t idx, float x, float y)
    {
        vertices.at(idx).Set(x, y);
    }

    size_t SdlVertexBufferBuilderService2D::GetNumVertices() const
    {
        return vertices.size();
    }

    VertexBufferBuilder2D& SdlVertexBufferBuilderService2D::Reset()
    {
        vertices.clear();
        return *this;
    }

    std::shared_ptr<VertexBuffer2D> SdlVertexBufferBuilderService2D::Build()
    {
        auto result = std::make_shared<SdlVertexBuffer2D>();
        result->vertices = vertices;
        return result;
    }

    /////////////////////////////////////////////////
    /////// SdlSceneGraph2
    /////////////////////////////////////////////////

    SdlSceneGraph2D::SdlSceneGraph2D(int renderPriority, int updatePriority)
        : Service("SDL Scene Graph 2D")
        , Updatable(updatePriority)
        , SdlRenderLayer(renderPriority)
    {
        // Intentionally left empty.
    }
    
    SdlSceneGraph2D::~SdlSceneGraph2D()
    {
        // Intentionally left empty.
    }

    void SdlSceneGraph2D::OnRender(SDL_Renderer* renderer)
    {
        sceneRenderer->SetViewMatrix( GetCamera().GetMatrix() );
        sceneRenderer->SetSdlRenderer( *renderer );
        GetRoot()->Render(*sceneRenderer, 1.0f);
        sceneRenderer->ClearSdlRenderer();
    }

    void SdlSceneGraph2D::OnUpdate()
    {
        GetRoot()->Update( GetElapsedTime() );
    }

    void SdlSceneGraph2D::OnStartup()
    {
        sceneRenderer = std::make_unique<SdlSceneRenderer2D>();
    }

    void SdlSceneGraph2D::OnShutdown()
    {
        sceneRenderer = nullptr;
    }

} // end of namespace
