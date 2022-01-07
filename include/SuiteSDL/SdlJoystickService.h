/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Service.h"
#include "Input/InputMappingService.h"

// C++ Standard Library includes
#include <memory>
#include <map>

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
         * 
         * @param defaultDeadZone   the default dead zone for control axis
         */
        SdlJoystickService(float defaultDeadZone = 0.25f);

        /**
         * Virtual destructor.
         */
        virtual ~SdlJoystickService();

        /**
         * Sets the deadzone of the specified axis control.
         * 
         * @param key   the axis control
         * @param deadzone  the deadzone within the range [0, 1]
         * @throws std::domain_error in case the deadzone is out of range
         */
        void SetAxisDeadzone(const Key& key, float deadzone);

        /**
         * Returns the deadzone for an axis control.
         * 
         * @return the deadzone
         */
        float GetAxisDeadzone(const Key& key) const;

    private:
        /** Used to update axis values of gamepads. */
        std::shared_ptr<InputMappingService> inputMapperSrv;

        /** Used to receive SDL events without exposing SDL in this header. */
        std::unique_ptr<EventListenerProxy> proxy;

        /** The deadzones for axis. */
        std::map<Key, float> deadzones;

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
    };

}