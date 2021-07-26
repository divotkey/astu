/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Resolution.h"
#include "Graphics/Color.h"

// C++ Standard Libryry includes
#include <string>

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
         * Sets the dimension of the window.
         * 
         * @param res   the window resolution
         */
        void SetSize(Resolution res) {
            switch (res) {
            case Resolution::SVGA:
                SetSize(800, 600);
                break;

            case Resolution::XGA:
                SetSize(1024, 768);
                break;

            case Resolution::WXGA:
                SetSize(1280, 720);
                break;

            case Resolution::SXGA:
                SetSize(1280, 1024);
                break;

            case Resolution::HD_1:
                SetSize(1360, 768);
                break;

            case Resolution::HD_2:
                SetSize(1366, 768);
                break;

            case Resolution::WXGA_PLUS:
                SetSize(1440, 900);
                break;

            case Resolution::WSXGA_PLUS:
                SetSize(1680, 1050);
                break;

            case Resolution::FHD:
                SetSize(1920, 1080);
                break;

            case Resolution::QHD:
                SetSize(2560, 1440);
                break;

            case Resolution::WIDE_SCREEN:
                SetSize(3440, 1440);
                break;

            case Resolution::UHD_4K:
                SetSize(3840, 2160);
            }
        }

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
         * Specifies whether the window should be resizable.
         * 
         * @param b the `true` if the window should be resizable
         */
        virtual void SetResizable(bool b) = 0;

        /**
         * Returns whether the window is resizable.
         * 
         * @return `true` if the window is resizable
         */
        virtual bool IsResizable() const = 0;
    
        /**
         * Returns the number of available displays.
         */
        virtual int NumDisplays() const = 0;

        /**
         * Acivates or deactivates fullscreen mode.
         * 
         * @param enableFullscreen  set to `true` to activate fullscreen mode
         */
        virtual void SetFullscreen(bool enableFullscreen) = 0;

        /**
         * Returns whether fullscreen mode is currently activated.
         * 
         * @return `true` if fulscreen mode is activated
         */
        virtual bool IsFullscreen() const = 0;
    };
    
} // end of namespace