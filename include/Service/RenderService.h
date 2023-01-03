/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Color.h"

namespace astu {

    /**
     * This interface is part of the graphics abstraction layer.
     * 
     * The graphics abstraction layer offers various possibilities for
     * rendering graphics output and provides various interfaces for this
     * (LineRenderer, SceneGraph, etc.). These interfaces are organized in
     * render layers and are managed by a central render mechanism. Access to 
     * this central render mechanism is provided by this interface.      
     * 
     * @ingroup srv_group
     */
    class RenderService {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~RenderService() {}

        /**
         * Returns the background color.
         * 
         * @return the current background color
         */
        virtual const Color4f& GetBackgroundColor() const = 0;
    
        /**
         * Sets the background color of the main application window.
         * 
         * @param bg    the background color
         */
        virtual void SetBackgroundColor(const Color4f& bg) = 0;

        /**
         * Returns the number of render layers.
         * 
         * @return the number of render layers
         */
        virtual size_t NumRenderLayers() const = 0;

        /**
         * Enables or disables vertical synchronization.
         * 
         * @param vsync set to `true` to enable vertical synchronization
         */
        virtual void SetVSync(bool vsync) = 0;

        /**
         * Returns whether vertical synchronization is enabled.
         * 
         * @return `true` if vertical synchronization is enabled
         */
        virtual bool IsVsync() const = 0;

    };
    
} // end of namespace