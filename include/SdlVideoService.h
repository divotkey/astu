/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes.
#include "Service.h"
#include "IWindowManager.h"

// C++ Standard Library includes
#include <string>

// Forward declaration.
struct SDL_Window;

namespace astu {

    /**
     * Initializes and opens the SDL-based main application window.
     * 
     * This service initializes the video submodule of SDL and maintains
     * the main application window.
     * 
     * @ingroup sdl_group
     */
    class SdlVideoService final 
        : public Service
        , public IWindowManager
    {
    public:

        /**
         * Constructor.
         */
        SdlVideoService();

        /**
         * Virtual destructor.
         */
        virtual ~SdlVideoService() {}

        /**
         * Returns a pointer to the SDL window structure.
         * 
         * This method is mostly used by other SDL services.
         * 
         * @return SDL_Window the SDL window or `nullptr` if this 
         *  service has not been started
         */
        SDL_Window* GetSdlWindow();

        /**
         * Returns whether Vulkan support has been enabled.
         * 
         * @return `true` if support for Vulkan has been enabled
         */
        bool IsVulkanSupportEnabled() const;

        /**
         * Enables or disables Vulkan support.
         * 
         * @param b set to `true` to enable Vulkan
         * @return reference to this service for method chaining
         * @throws std::logic_error in case this service is already running
         */
        SdlVideoService& EnableVulkanSupport(bool b);

        // Inherited via IWindowManager
        virtual void SetSize(int width, int height) override;
        virtual int GetWidth() const override;
        virtual int GetHeight() const override;
        virtual void SetTitle(const std::string & title) override;
        virtual const std::string & GetTitle() const override;
        virtual void SetResizeable(bool b) override;
        virtual bool IsResizeable() const override;

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** The SDL window. */
        SDL_Window* window;

        /** The window width. */
        int winWidth;

        /** The window height. */
        int winHeight;

        /** The window title. */
        std::string winTitle;

        /** Whether to enable support for Vulkan. */
        bool vulkanSupport;

        /** Whether the window should be resizable. */
        bool resizeable;

        /**
         * Releases recources. 
         */
        void CleanUp();
    };

} 