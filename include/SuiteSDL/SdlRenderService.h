/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>
#include <vector>

// Local includes
#include "Service/UpdateService.h"
#include "Service/RenderService.h"
#include "Color.h"

// Forward declaration.
struct SDL_Renderer;

namespace astu {

    /**
     * Interface for SDL-based renderer layers.
     * 
     * This inteface for render layers is using the hardware accelerated 2D
     * render-mechanism of SDL. 
     * 
     * @ingroup sdl_group
     */
    class ISdlRenderLayer {
    public:

        /**
         * Virtual destructor.
         */
        ~ISdlRenderLayer() {}

        /**
         * Called by the render service to render this layer.
         * 
         * @param renderer  the SDL renderer
         */
        virtual void OnRender(SDL_Renderer* renderer) = 0;

        /**
         * Called when the size of the render target has changed.
         * 
         * It is guaranteed that this method is called at least once before
         * the first call of `OnRender`.
         * 
         * @param width the width of the render target in pixels
         * @param height the height of the render target in pixels
         */
        virtual void OnResize(int width, int height) = 0;

        /**
         * Returns the render priority.
         * Lower priorities get rendered before higher priorities.
         * 
         * @return the render priority
         */
        virtual int GetRenderPriority() const = 0;
    };


    /**
     * Initializes and maintains an SDL renderer.
     * 
     * This service has a depencendy to SdlVideoService because
     * creating an SDL renderer requires access to the SDL window.
     * 
     * @ingroup sdl_group
     */
    class SdlRenderService final 
        : public virtual Service
        , private Updatable
        , public RenderService
    {
    public:

        /**
         * Constructor.
         * 
         * @param priority    the priority used to update this service
         */
        SdlRenderService(int priority = Priority::Normal);

        /**
         * Virtual destructor.
         */
        virtual ~SdlRenderService() {}

        /**
         * Adds a render layer to this service.
         * 
         * @param layer the render layer to add
         */
        void AddLayer(ISdlRenderLayer &layer);
        
        /**
         * Removes a render layer from this service.
         * 
         * @param layer the render layer to remove
         */
        void RemoveLayer(ISdlRenderLayer &layer);

        /**
         * Tests whether a render layer has already been added.
         * 
         * @param layer the render to test
         * @return `true` if the layer has already been added
         */
        bool HasLayer(ISdlRenderLayer &layer);

        // INherited via RenderService
        virtual void SetBackgroundColor(const Color4f& bg) override;
        virtual const Color4f& GetBackgroundColor() const override;

    private:
        /** The SDL renderer. */
        SDL_Renderer* renderer;

        /** The render layers. */
        std::vector<ISdlRenderLayer*> layers;

        /** The background color. */
        Color4f backgroundColor;

        /**
         * Logs some additional information about the used renderer.
         */
        void LogRendererInfo();

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via Updatable
        virtual void OnUpdate() override;
    };

    /**
     * Services can derive from this class to become a SDL render layer.
     * 
     * @ingroup sdl_group
     */
    class SdlRenderLayer 
        : public virtual Service
        , public ISdlRenderLayer
    {
    public:

        /**
         * Constructor.
         * 
         * @param renderPriority    the render priority of this layer
         */
        SdlRenderLayer(int renderPriority);

        /**
         * Virtual destructor.
         */
        virtual ~SdlRenderLayer() {}

        // Inherited via ISdlRenderLayer.
        virtual int GetRenderPriority() const final override;
        virtual void OnResize(int width, int height) override;

        /**
         * Returns the width of the render target.
         * 
         * @return the width in pixels
         */
        int GetTargetWidth() const {
            return targetWidth;
        }

        /**
         * Returns the height of the render target.
         * 
         * @return the height in pixels
         */
        int GetTargetHeight() const {
            return targetHeight;
        }

    private:
        /** The render priority of this render layer. */
        int renderPriority;

        /** The width of the render target in pixels. */
        int targetWidth;

        /** The height of the render target in pixels. */
        int targetHeight;
    };

} // end of namespace