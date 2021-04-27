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

    class TimeClient : virtual Service {
    public:

        TimeClient() {
            AddStartupHook([this]() { timeSrv = ASTU_GET_SERVICE(ITimeManager); });
            AddShutdownHook([this]() { timeSrv = nullptr; });
        }

    protected:

        virtual ~TimeClient() {
        }

        double GetElapsedTime() const {
            return timeSrv->GetElapsedTime();
        }

        double GetAbsoluteTime() const {
            return timeSrv->GetAbsoluteTime();
        }

    private:
        std::shared_ptr<ITimeManager> timeSrv;
    };

} // end of namespace