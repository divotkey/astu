/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Service.h"
#include "ServiceManager.h"

// C++ Standard Library includes
#include <stdexcept>


namespace astu {

    /////////////////////////////////////////////////
    /////// Service
    /////////////////////////////////////////////////

    const std::string & Service::DEFAULT_NAME = "UNNAMED";

    Service::Service(const std::string & name)
        : name(name)
        , status(Stopped)
    {
        // Intentionally left empty.
    }

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

} // end of namespace