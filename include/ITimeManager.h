/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stdexcept>

// Local includes
#include "SignalService.h"
#include "Service.h"
#include "Events.h"

namespace astu {

    /**
     * Interface for services which keeps track of time.
     * 
     * @ingroup srv_group
     */
    class ITimeManager {
    public:

        /**
         * Virtual destructor.
         */
        ~ITimeManager() {}

        /**
         * Returns the elapsed time since the last update.
         * 
         * @return the elapsed time in seconds
         */
        virtual double GetElapsedTime() const = 0;

        /**
         * Return the absolute time since the service has been started.
         * 
         * @return the absolute time
         */
        virtual double GetAbsoluteTime() const = 0;
    };

    /**
     * Services can derive from this if access to elapsed and absolute time is 
     * needed.
     */
    class TimeClient : virtual Service {
    public:

        /**
         * Constructor.
         */
        TimeClient() {
            AddStartupHook([this]() { timeSrv = ASTU_GET_SERVICE(ITimeManager); });
            AddShutdownHook([this]() { timeSrv = nullptr; });
        }

        /** Virtual destructor. */
        virtual ~TimeClient() { }

    protected:

        /**
         * Returns the elapsed time since the last update.
         * 
         * @return the elapsed time in seconds
         */
        double GetElapsedTime() const {
            return timeSrv->GetElapsedTime();
        }

        /**
         * Returns the elapsed time since the last update in single precision.
         * 
         * @return the elapsed time in seconds
         */
        float GetElapsedTimeF() const {
            return static_cast<float>(timeSrv->GetElapsedTime());
        }

        /**
         * Returns the absolute time since.
         * 
         * @return the absolute time in seconds
         */
        double GetAbsoluteTime() const {
            return timeSrv->GetAbsoluteTime();
        }

    private:
        /** The time manager used by this class. */
        std::shared_ptr<ITimeManager> timeSrv;
    };

} // end of namespace