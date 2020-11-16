/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include <vector>
#include "Color.h"
#include "UpdateService.h"

// Forward declaration.
struct SDL_Renderer;

namespace astu {

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
     */
    class SdlRenderService : public UpdatableBaseService {
    public:

        /**
         * Constructor.
         */
        SdlRenderService();

        /**
         * Virtual destructor.
         */
        virtual ~SdlRenderService() {}

        /**
         * Adds a render layer to this service.
         * 
         * @param layer the render layer to add
         */
        void AddLayer(std::shared_ptr<ISdlRenderLayer> layer);
        
        /**
         * Removes a render layer from this service.
         * 
         * @param layer the render layer to remove
         */
        void RemoveLayer(std::shared_ptr<ISdlRenderLayer> layer);

        /**
         * Tests whether a render layer has already been added.
         * 
         * @param layer the render to test
         * @return `true` if the layer has already been added
         */
        bool HasLayer(std::shared_ptr<ISdlRenderLayer> layer);

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
        virtual void OnUpdate() override;

    private:
        /** The SDL renderer. */
        SDL_Renderer* renderer;

        /** The render layers. */
        std::vector<std::shared_ptr<ISdlRenderLayer>> layers;

        /** The backtround color. */
        Color backgroundColor;

        /**
         * Logs some additional information about the used renderer.
         */
        void LogRendererInfo();
    };

    /**
     * Base class for services which implement the ISdlRenderLayer interface.
     */
    class BaseSdlRenderLayer 
        : public BaseService
        , public ISdlRenderLayer
        , public std::enable_shared_from_this<BaseSdlRenderLayer> 
    {
    public:

        /**
         * Constructor.
         * 
         * @param renderPriority    the render priority of this layer
         * @param name              the name of this service
         */
        BaseSdlRenderLayer(int renderPriority, const std::string & name = BaseService::DEFAULT_NAME);

        /**
         * Virtual destructor.
         */
        virtual ~BaseSdlRenderLayer() {}


        // Inherited via BaseService/IService
        virtual void Startup() override;
        virtual void Shutdown() override;

        // Inherited via ISdlRenderLayer.
        virtual int GetRenderPriority() const final override;
        virtual void OnResize(int width, int height) override;

        /**
         * Returns the width of the render taret.
         * 
         * @return the width in pixels
         */
        int GetTargetWidth() const {
            return targetWidth;
        }

        /**
         * Returns the height of the render taret.
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