/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service.h"
#include "InputMappingService.h"

// C++ Standard Library includes
#include <memory>

namespace astu {

    // Forward declaration.
    class EventListenerProxy;

    /**
     * Uses activates the SDL joystick subsystem and processes the events.
     * This service needs to be added in order to use gamepads etc.
     * 
     * @ingroup sdl_group 
     */
    class SdlJoystickService final 
        : public virtual Service
    {
    public:

        /**
         * Constructor.
         */
        SdlJoystickService();

        /**
         * Virtual destructor.
         */
        virtual ~SdlJoystickService();

    private:
        /** Used to update axis values of gamepads. */
        std::shared_ptr<InputMappingService> inputMapperSrv;

        /** Used to receive SDL events without exposing SDL in this header. */
        std::unique_ptr<EventListenerProxy> proxy;

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
    };

}