/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Libryry includes
#include <string>

// Local includes
#include "Graphics/Color.h"

namespace astu {

    /**
     * Interface for an services which manages application windows.
     * 
     * @ingroup srv_group
     */
    class WindowService {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~WindowService() {}

        /**
         * Sets the dimension of the window.
         * 
         * @param width the width of the window in pixels
         * @param height the height of the window in pixels
         */
        virtual void SetSize(int width, int height) = 0;

        /**
         * Returns the width of the window in pixels.
         * 
         * @return the width in pixls
         */
        virtual int GetWidth() const = 0;

        /**
         * Returns the height of the window in pixels.
         * 
         * @return the height in pixls
         */
        virtual int GetHeight() const = 0;

        /**
         * Sets the window title.
         * 
         * @param title the title of the window
         */
        virtual void SetTitle(const std::string & title) = 0;

        /**
         * Returns the current window title.
         * 
         * @return the window title
         */
        virtual const std::string & GetTitle() const = 0;

        /***
         * Specifies whether the window should be resizeable.
         * 
         * @param b the `true` if the window should be resizeable
         */
        virtual void SetResizeable(bool b) = 0;

        /**
         * Returns whether the window is resizeable.
         * 
         * @return `true` if the window is resizeable
         */
        virtual bool IsResizeable() const = 0;
    };
    
} // end of namespace