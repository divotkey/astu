/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * Interface to listeners to SDL-internal resize events.
     */
    class ISdlResizeListener {
    public:
    
        /** Virtual destructor. */
        virtual ~ISdlResizeListener() {}

        /**
         * Called to handle an SDL resize event.
         * 
         * @param width the new width of the application window
         * @param height the new height of the application window
         */
        virtual void OnResize(int width, int height) = 0;
    };

} // end of namespace
