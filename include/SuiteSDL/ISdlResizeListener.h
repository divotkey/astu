/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
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
         * @param   the SDL resize event
         */
        virtual void OnResize(int width, int height) = 0;
    };

} // end of namespace
