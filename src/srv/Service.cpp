/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include "ServiceManager.h"
#include "Service.h"

namespace astu {

    const std::string & BaseService::DEFAULT_NAME = "NO NAME";

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
        OnStartup();
        running = true;
    }

    void BaseService::Shutdown()
    {
        // Best practice: ignore shut down calls on not-running services.
        if (IsRunning()) {
            OnShutdown();        
            running = false;
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



} // end of namespace