/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/SignalService.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <stdexcept>

namespace astu {

    /**
     * This abstract base class allows to query the current time (absolute and elapsed).
     * 
     * @ingroup srv_group
     */
    class TimeService {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~TimeService() {}

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
     * Services can derive from this class to have easy access to time service.
     * 
     * @ingroup srv_group
     */
    class TimeClient : virtual Service {
    public:

        /**
         * Constructor.
         */
        TimeClient() {
            AddStartupHook([this]() { timeSrv = ASTU_GET_SERVICE(TimeService); });
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
        std::shared_ptr<TimeService> timeSrv;
    };

} // end of namespace