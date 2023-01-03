/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "SdlRenderService.h"
#include "Math/Vector2.h"
#include "Suite2D/Scene.h"
#include "Graphics/VertexBuffer2.h"
#include "Graphics/Texture.h"
#include "Suite2D/CameraService.h"
#include "Service/TimeService.h"
#include "Service/UpdateService.h"

// C++ Standard libraries includes
#include <memory>
#include <vector>

namespace astu {

    // Forward declaration
    class SdlVertexBuffer2D;
    class SdlSceneRenderer2D;

    /**
     * Required for the SdlSceneGraph2D service.
     * 
     * @ingroup sdl_group 
     */
    class SdlVertexBufferBuilderService2D 
        : virtual public Service
        , public VertexBufferBuilder2<float>
    {
    public:

        /**
         * Constructor.
         */
        SdlVertexBufferBuilderService2D();

        // Inherited via VertexBufferBuilder2f
        virtual VertexBufferBuilder2f& AddVertex(float x, float y) override;
        virtual const Vector2f& GetVertex(size_t idx) const override;
        virtual VertexBufferBuilder2f& SetVertex(size_t idx, float x, float y) override;
        virtual size_t GetNumVertices() const override;
        virtual VertexBufferBuilder2f& Reset() override;
        virtual std::shared_ptr<VertexBuffer2f> Build() override;        

    private:
        /** The vertices used for the buffer to build. */
        std::vector<astu::Vector2f> vertices;
    };

    /**
     * Required for the SdlSceneGraph2D service.
     *
     * @ingroup sdl_group
     */
    class SdlTextureFactoryService
            : virtual public Service
            , public TextureFactory
    {
    public:

        /**
         * Constructor.
         */
        SdlTextureFactoryService();

        // Inherited via TextureFactory
        virtual std::shared_ptr<Texture> CreateFromImage(const Image &image, ScaleQuality sq) const override;
        virtual std::shared_ptr<Texture> CreateFromBmp(const std::string& filepath, ScaleQuality sq) const override;
    };

    /**
     * Provides a SDL-based implementation of the 2D scene graph.
     * 
     * @ingroup sdl_group 
     */
    class SdlSceneGraph2D final 
        : public SdlRenderLayer 
        , public Updatable
        , public TimeClient
        , public suite2d::SceneGraph
        , public suite2d::CameraClient
    {
    public:

        /**
         * Constructor.
         * 
         * @param renderPriority    the priority of this render layer
         * @param updatePriority    the update priority of this scene graph
         */
        SdlSceneGraph2D(
            int renderPriority = Priority::Normal, 
            int updatePriority = Priority::Low);

        /** Virtual destructor. */
        virtual ~SdlSceneGraph2D();

        // Inherited via Updatable
        virtual void OnUpdate() override;

        // Inherited via SdlRenderLayer
        virtual void OnRender(SDL_Renderer* renderer) override;
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** The scene renderer used to render the scene graph. */
        std::unique_ptr<SdlSceneRenderer2D> sceneRenderer;
    };

} // end of namespace