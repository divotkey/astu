/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Color.h"
#include "Service/Resolution.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <string>

namespace astu {

    // Forward declaration
    class UpdateService;

    /**
     * Base class for interactive applications.
     * 
     * @ingroup srv_group
     */
    class InteractiveApplication 
        : public IWindowStateListener
        , public IKeystrokeListener
    {
    public:

        /**
         * Constructor.
         */
        InteractiveApplication();

        /**
         * Virtual destructor.
         */
        ~InteractiveApplication() {}

        /**
         * Returns version information as string.
         * 
         * @return the version information of this application
         */
        const std::string& GetVersionString() const;

        /**
         * Defines whether the application terminates on window-close signals.
         * 
         * @param b set to `true` if the application should terminate
         */
        void EnableTerminateOnClose(bool b);

        /**
         * Returns whether the applicaiton terminates on window-close signals.
         * 
         * @return `true` if the application terminates on window-close signals
         */
        bool IsTerminateOnCloseEnabled() const;

        /**
         * Defines whether the application terminates when ESC key is pressed.
         * 
         * @param b set to `true` if the application should terminate
         */
        void EnableTerminateOnEscape(bool b);

        /**
         * Returns whether the applicaiton terminates when ESC key is pressed.
         * 
         * @return `true` if the application terminates
         */
        bool IsTerminateOnEscapeEnabled() const;

        /**
         * Sets the version information string for this application.
         * 
         * @param version   the version information string
         */
        void SetVersionString(const std::string& version);

        /**
         * Returns name of this application.
         * 
         * @return the application name
         */
        const std::string& GetApplicationName() const;

        /**
         * Sets the name of this application.
         * 
         * @param name  the application name
         */
        void SetApplicationName(const std::string& name);

        /**
         * Returns a application information e.g., name, version etc.
         * 
         * @return a string containing application information
         */
        std::string GetInfoString() const;

        /**
         * Defines the resolution of the application window on startup.
         * 
         * @param res   the resolution
         */
        void SetResolution(Resolution res);

        /**
         * Returns the startup resolution.
         * 
         * @return the initial resolution of the application window
         */
        Resolution GetResolution() const;

        /**
         * Defines whether the application should start in fullscreen mode.
         * 
         * @param startFullscreen   `true` to start in fullscreen mode
         */
        void SetFullscreen(bool startFullscreen);

        /**
         * Returns whether the application starts in fullscreen mode.
         * 
         * @return `true` if the application starts in fullscreen
         */
        bool IsFullscreen() const;

        /**
         * Defines whether the main application window should be resizable.
         * 
         * @param resizable set to `true` if the window should be resizeable
         */
        void SetResizable(bool resizable);

        /**
         * Returns whether the main applicaiton window can be resized.
         * 
         * @return `true` if the application window can be resized
         */
        bool IsResizable() const;

        /**
         * Sets the key used to toggle fullscreen mode.
         * 
         * @param keycode   the key
         */
        void SetFullscreenToggleKey(int keycode);

        /**
         * Returns the key used to toggle fullscreen mode.
         * 
         * @return the key
         */
        int GetFullscreenToggleKey() const;

        /**
         * Sets the background color of the application window on startup.
         * 
         * If the application is already running, calling this method will
         * not change the current background color of the application window.
         * 
         * @param color the background color
         */
        void SetBackgroundColor(const Color4f& color);

        /**
         * Returns the background color used on startup.
         * 
         * This method will not query the currently used background color of
         * the application window but the background color to be used on 
         * startup.
         * 
         * @return the background color
         */
        const Color4f& GetBackgroundColor() const;

        /**
         * Terminates the application at the beginning of the next main cycle.
         */
        void ScheduleTermination();
    
        /**
         * Starts all services and runs the main loop of this application.
         */
        virtual void Run();

    protected:
        /**
         * Configures services according to application specific settings.
         */
        virtual void ConfigureApplication();

        /**
         * Releases resources, de-registers as listener etc.
         */
        virtual void Cleanup();

    private:

        /**
         * Adds required core services.         * 
         */
        void AddCoreServices();

        /** Indicates whether this application should terminate. */
        bool terminated;

        /** Contains version information about this application. */
        std::string versionString;

        /** The name of this application. */
        std::string appName;

        /** The background color of this application. */
        Color4f backgroundColor;

        /** Whether the application should terminate on window close signal. */
        bool terminateOnClose;
        
        /** Whether the application should terminate when ESC key is pressed. */
        bool terminateOnEscape;

        /** Whether version information should be printed to the terminal. */
        bool printVersionInfo;

        /** The startup resolution of the application window. */
        Resolution startupResolution;

        /** Whetehr the application should start in fullscreen mode. */
        bool fullscreen;

        /** The key used to toggle fullscreen mode. */
        int fullScreenKey;

        /** Whether the application window should be resizeable. */
        bool resizable;

        /** Whether the application is currently running. */
        bool running;

        /**
         * Prints version information to the terminal.
         */
        void PrintVersionInfo();

        // Inherited via IWindowStateListener
        virtual bool OnSignal(const WindowState& signal) override;

        // Inherited via IKeystrokeListener
        virtual bool OnSignal(const KeystrokeSignal& signal) override;        
    };

} // end of namespace