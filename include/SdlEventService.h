/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "InputMappingService.h"
#include "UpdateService.h"
#include "Keyboard.h"
#include "Events.h"
#include "Mouse.h"

namespace astu {

    // Forward declaration
    class ISdlEventListener;

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
        SdlEventService(int priority = Priority::Normal);

        /**
         * Virtual destructor.
         */
        virtual ~SdlEventService() {}

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

        /**
         * Adds an event listener which listens to raw sdl events.
         * This method is for internal usage only.
         * 
         * @param listener  event listener to add
         * @throws std::logic_error in case the listener has already been added
         */
        void AddSdlEventListener(ISdlEventListener & listener);

        /**
         * Removes an event listener.
         * This method is for internal usage only.
         * 
         * @param listener  event listener to remove
         */
        void RemoveSdlEventListener(ISdlEventListener & listener);

        /**
         * Tests whether an event listener has already been added.
         * This method is for internal usage only.
         * 
         * @param listener  event listener to remove
         */
        bool HasSdlEventListener(ISdlEventListener & listener) const;

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

        /** Used to map input events to actions and axis. */
        std::shared_ptr<InputMappingService> inputMapperSrv;

        /** Used to transmit mouse button events. */
        std::shared_ptr<MouseButtonEventService> mouseButtonSrv;

        /** Used to transmit mouse wheel events. */
        std::shared_ptr<MouseWheelEventService> mouseWheelSrv;

        /** Used to transmit mouse move events. */
        std::shared_ptr<MouseMoveEventService> mouseMoveSrv;

        /** Used to transmit keystroke events. */
        std::shared_ptr<KeystrokeEventService> keystrokeSrv;

        /** Used to transmit keystroke events. */
        std::shared_ptr<ResizeEventService> resizeSrv;

        /** The registered event listeners. */
        std::vector<ISdlEventListener*> eventListeners;
    };

} 