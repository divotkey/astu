/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard libraries includes
#include <memory>
#include <vector>

// Local includes
#include "Scene2.h"
#include "Vector2.h"
#include "Service.h"
#include "ITimeManager.h"
#include "UpdateService.h"
#include "VertexBuffer2.h"
#include "CameraService2D.h"
#include "SdlRenderService.h"

namespace astu {

    // Forward declaration
    class SdlVertexBuffer2;
    class SdlScene2Renderer;

    class SdlVertexBuffer2BuilderService 
        : public Service
        , public VertexBuffer2Builder
    {
    public:

        /**
         * Constructor.
         */
        SdlVertexBuffer2BuilderService();

        // Inherited via VertexBuffer2Builder
        virtual VertexBuffer2Builder& AddVertex(float x, float y) override;
        virtual Vector2f GetVertex(size_t idx) const override;
        virtual void SetVertex(size_t idx, float x, float y) override;
        virtual size_t GetNumVertices() const override;
        virtual VertexBuffer2Builder& Reset() override;
        virtual std::shared_ptr<VertexBuffer2> Build() override;        

    private:
        /** The vertices used for the buffer to build. */
        std::vector<astu::Vector2f> vertices;
    };

    class SdlSceneGraph2 final 
        : public SdlRenderLayer 
        , public Updatable
        , public TimeClient
        , public SceneGraph2
        , public CameraClient2D
    {
    public:

        /**
         * Constructor.
         * 
         * @param renderPriority    the priority of this render layer
         * @param updatePriority    the update priority of this scene graph
         */
        SdlSceneGraph2(
            int renderPriority = Priority::Normal, 
            int updatePriority = Priority::Low);

        /** Virtual destructor. */
        virtual ~SdlSceneGraph2();

        // Inherited via Updatable
        virtual void OnUpdate() override;

        // Inherited via SdlRenderLayer
        virtual void OnRender(SDL_Renderer* renderer) override;
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** The scene renderer used to render the scene graph. */
        std::unique_ptr<SdlScene2Renderer> sceneRenderer;
    };

} // end of namespace