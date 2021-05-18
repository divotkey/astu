/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "UpdateService.h"
#include "Keyboard.h"
#include "Events.h"
#include "Mouse.h"

namespace astu {

    /**
     * Initializes the SDL event submodule. 
     * 
     * This service processes the SDL event queue and distributes the events through
     * the various abstractions layers (input, window-handling, etc.).
     *
     * @ingroup sdl_group
     */
    class SdlEventService final : public virtual Service, public Updatable {
    public:

        /**
         * Constructor.
         * 
         * @param priority    the priority used to update this service
         */
        SdlEventService(int priority = 0);

        /**
         * Virtual destructor.
         */
        ~SdlEventService() {}

        /**
         * Return whether a quit-signal has been detected.
         * 
         * @return `true` if a quit-signal has been detected
         */
        bool IsQuit() const;

        /**
         * Clears the quit-signal if set.
         */
        void ClearQuit();

    protected:

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via Updatable
        virtual void OnUpdate() override;

    private:
        /** Determines whether a quit-signal has been detected. */
        bool quit;

        /** Used to transfer mouse events to mouse states. */
        Mouse mouse;

        /** Used to transfer keyboard events to keyboard states. */
        Keyboard keyboard;

        /** Used to transmit mouse button events. */
        std::shared_ptr<MouseButtonEventService> mouseButtonSrv;

        /** Used to transmit keystroke events. */
        std::shared_ptr<KeystrokeEventService> keystrokeSrv;

        /** Used to transmit keystroke events. */
        std::shared_ptr<ResizeEventService> resizeSrv;
    };

} 