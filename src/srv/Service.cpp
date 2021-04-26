/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <stdexcept>

// Local includes
#include "ServiceManager.h"
#include "SignalService.h"
#include "Service.h"
#include "Events.h"

namespace astu {

    const std::string & BaseService::DEFAULT_NAME = "UNNAMED";

    BaseService::BaseService(const std::string & aName)
        : name(aName)
        , running(false)
    {
        // Intentionally left empty
    }

    const std::string & BaseService::GetName() const
    {
        return name;
    }

    void BaseService::Startup()
    {
        if (IsRunning()) {
            throw std::logic_error("Service " + GetName() + " already running");
        }

        // START OF EXPERIMENTAL CODE
        for (auto & hook : startupHooks) {
            hook();
        }
        // END OF EXPERIMENTAL CODE

        OnStartup();
        running = true;
        auto srvEvents = GetSM().FindServiceOrNull<SignalService<ServiceEvent>>();
        if (srvEvents) {
            srvEvents->FireSignal(ServiceEvent(ServiceEvent::Started, *this));
        }
    }

    void BaseService::Shutdown()
    {
        // Best practice: ignore shut down calls on not-running services.
        if (IsRunning()) {
            OnShutdown();

            // START OF EXPERIMENTAL CODE
            for (auto & hook : shutdownHooks) {
                hook();
            }
            // END OF EXPERIMENTAL CODE

            running = false;

            auto srvEvents = GetSM().FindService<SignalService<ServiceEvent>>();
            if (srvEvents) {
                srvEvents->FireSignal(ServiceEvent(ServiceEvent::Stopped, *this));
            }
        }
    }

    bool BaseService::IsRunning() const
    {
        return running;
    }

    ServiceManager & BaseService::GetSM()
    {
        return ServiceManager::GetInstance();
    }

    void BaseService::AddStartupHook(std::function<void (void)> func)
    {
        startupHooks.push_back(func);
    }

    void BaseService::AddShutdownHook(std::function<void (void)> func)
    {
        shutdownHooks.push_back(func);
    }


} // end of namespace