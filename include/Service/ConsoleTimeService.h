/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/UpdateService.h"
#include "Service/TimeService.h"

// C++ Standard Library includes
#include <chrono>

namespace astu {

    /**
     * Uses the C++ Standard Library chronos API to measure elapsed time.
     *
     * @ingroup sdl_group
     */
    class ConsoleTimeService final
        : public virtual Service
        , private Updatable
        , public TimeService
    {
    public:

        /**
         * Constructor.
         *
         * @param priority    the priority used to update this service
         */
        ConsoleTimeService(int priority = Priority::VeryHigh);

        /**
         * Virtual destructor.
         */
        virtual ~ConsoleTimeService() {}

        // Inherited via TimeService
        virtual double GetElapsedTime() const override;
        virtual double GetAbsoluteTime() const override;

    protected:

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via Updatable
        virtual void OnUpdate() override;

    private:
        /** Used to measure elapsed time since last update. */
        std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;

        /** Represents the absolute time since this service has been started. */
        std::chrono::nanoseconds timeInNs;

        /** use to avoid issues while debugging or drop of update rate. */
        double maxDeltaTime;

        /** The elapsed time since the last update. */
        double deltaTime;

        /** The absolute time. */
        double time;
    };


} // end of namespace



