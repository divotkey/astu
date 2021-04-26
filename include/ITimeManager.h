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

    class TimeClient : private IServiceListener {
    public:

        TimeClient() {
            ASTU_SERVICE(ServiceEventService).AddListener(this);
        }

        virtual ~TimeClient() {
            ASTU_SERVICE(ServiceEventService).RemoveListener(this);
        }

        double GetElapsedTime() const {
            return timeSrv->GetElapsedTime();
        }

        double GetAbsoluteTime() const {
            return timeSrv->GetAbsoluteTime();
        }

    private:
        std::shared_ptr<ITimeManager> timeSrv;

        // Inherited via IServiceListener
        virtual void OnSignal(const ServiceEvent & event) override {

            if (dynamic_cast<TimeClient*>(&event.service) == this) {

                switch (event.type) {
                case ServiceEvent::Started:
                    timeSrv = ServiceManager::GetInstance().FindService<ITimeManager>();
                    if (!timeSrv) {
                        throw std::logic_error("Time service not present");
                    }
                    break;

                case ServiceEvent::Stopped:
                    timeSrv = nullptr;
                    break;
                }
            }
        }
    };

} 