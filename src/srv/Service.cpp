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

    void Service::AddPauseHook(std::function<void (void)> func)
    {
        pauseHooks.push_back(func);
    }

    void Service::AddResumeHook(std::function<void (void)> func)
    {
        resumeHooks.push_back(func);
    }

    void Service::Startup()
    {
        if (status != Stopped) {
            throw std::logic_error("Service " + GetName() + " already running");
        }

        // Call all startup hooks.
        for (auto & hook : startupHooks) {
            hook();
        }

        // Call startup on primary derived class.
        OnStartup();

        // Update status.
        status = Running;
    }

    void Service::Shutdown()
    {
        // Best practice is to  ignore shut down calls on stopped services.
        if (status == Stopped)
            return;

        // Call shutdown on primary derived class.
        OnShutdown();

        // Call all shutdown hooks.
        for (auto & hook : shutdownHooks) {
            hook();
        }

        // Update status.
        status = Stopped;
    }

    void Service::Pause()
    {
        if (status != Running) {
            throw std::logic_error("Service " + GetName() + " not running");
        }

        // Call all pause hooks.
        for (auto & hook : pauseHooks) {
            hook();
        }

        // Call pause on primary derived class.
        OnPaused();

        // Update status.
        status = Paused;
    }

    bool Service::IsPaused() const
    {
        return status == Paused;
    }

    void Service::Resume()
    {
        // Best practice is to ignore resume calls on running services.
        if (status == Running)
            return;

        if (status != Paused) {
            throw std::logic_error("Unable to resume service " 
                + GetName() + " not running");
        }

        // Call all resume hooks.
        for (auto & hook : resumeHooks) {
            hook();
        }

        // Call resume on primary derived class.
        OnResumed();

        // Update status.
        status = Running;
    }

} // end of namespace