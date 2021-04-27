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
#include "Events.h"
#include "Service.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// Service
    /////////////////////////////////////////////////

    const std::string & Service::DEFAULT_NAME = "UNNAMED";

    Service::~Service()
    {
        // Intentionally left empty.
    }

    const std::string & Service::GetName() const
    {
        return name;
    }

    Service::Status Service::GetStatus() const
    {
        return status;
    }

    void Service::AddStartupHook(std::function<void (void)> func)
    {
        startupHooks.push_back(func);
    }

    void Service::AddShutdownHook(std::function<void (void)> func)
    {
        shutdownHooks.push_back(func);
    }

    void Service::Startup()
    {
        if (status != Stopped) {
            throw std::logic_error("Service " + GetName() + " already running");
        }

        for (auto & hook : startupHooks) {
            hook();
        }

        OnStartup();
        status = Running;
    }

    void Service::Shutdown()
    {
        // Best practice is to  ignore shut down calls on stopped services.
        if (status == Stopped)
            return;

        OnShutdown();

        for (auto & hook : shutdownHooks) {
            hook();
        }

        status = Stopped;
    }


    /////////////////////////////////////////////////
    /////// Other things
    /////////////////////////////////////////////////

    // const std::string & BaseService::DEFAULT_NAME = "UNNAMED";

    // BaseService::BaseService(const std::string & aName)
    //     : name(aName)
    //     , running(false)
    // {
    //     // Intentionally left empty
    // }

    // const std::string & BaseService::GetName() const
    // {
    //     return name;
    // }

    // void BaseService::Startup()
    // {
    //     if (IsRunning()) {
    //         throw std::logic_error("Service " + GetName() + " already running");
    //     }

    //     // START OF EXPERIMENTAL CODE
    //     for (auto & hook : startupHooks) {
    //         hook();
    //     }
    //     // END OF EXPERIMENTAL CODE

    //     OnStartup();
    //     running = true;
    //     auto srvEvents = ASTU_GET_SERVICE_OR_NULL(SignalService<ServiceEvent>);
    //     if (srvEvents) {
    //         srvEvents->FireSignal(ServiceEvent(ServiceEvent::Started, *this));
    //     }
    // }

    // void BaseService::Shutdown()
    // {
    //     // Best practice: ignore shut down calls on not-running services.
    //     if (IsRunning()) {
    //         OnShutdown();

    //         // START OF EXPERIMENTAL CODE
    //         for (auto & hook : shutdownHooks) {
    //             hook();
    //         }
    //         // END OF EXPERIMENTAL CODE

    //         running = false;


    //         auto srvEvents = ASTU_GET_SERVICE_OR_NULL(SignalService<ServiceEvent>);
    //         if (srvEvents) {
    //             srvEvents->FireSignal(ServiceEvent(ServiceEvent::Stopped, *this));
    //         }
    //     }
    // }

    // bool BaseService::IsRunning() const
    // {
    //     return running;
    // }

    // ServiceManager & BaseService::GetSM()
    // {
    //     return ServiceManager::GetInstance();
    // }

    // void BaseService::AddStartupHook(std::function<void (void)> func)
    // {
    //     startupHooks.push_back(func);
    // }

    // void BaseService::AddShutdownHook(std::function<void (void)> func)
    // {
    //     shutdownHooks.push_back(func);
    // }


} // end of namespace