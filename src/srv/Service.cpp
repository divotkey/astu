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

        for (auto & hook : startupHooks) {
            hook();
        }

        OnStartup();
        running = true;
        auto srvEvents = GetSM().FindService<SignalService<ServiceEvent>>();
        if (srvEvents) {
            srvEvents->FireSignal(ServiceEvent(ServiceEvent::Started, *this));
        }

        // GetSM().FireServiceStarted(*this);
    }

    void BaseService::Shutdown()
    {
        // Best practice: ignore shut down calls on not-running services.
        if (IsRunning()) {
            OnShutdown();

            for (auto & hook : shutdownHooks) {
                hook();
            }

            running = false;

            auto srvEvents = GetSM().FindService<SignalService<ServiceEvent>>();
            if (srvEvents) {
                srvEvents->FireSignal(ServiceEvent(ServiceEvent::Stopped, *this));
            }
            // GetSM().FireServiceStopped(*this);
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