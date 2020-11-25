/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <algorithm>
#include "Service.h"
#include "ServiceManager.h"

namespace astu {

    ServiceManager * ServiceManager::theOneAndOnly = new ServiceManager();

    ServiceManager & ServiceManager::GetInstance()
    {
        return *theOneAndOnly;
    }

    ServiceManager::ServiceManager()
        : running(false)
    {
        // Intentionally left empty.
    }

    void ServiceManager::AddService(std::shared_ptr<IService> service)
    {
        if (HasService(service)) {
            throw std::logic_error("Service '" + service->GetName() 
                + "' has already been added");
        }

        services.push_back(service);

        // Start service if all other service are currently running.
        if (running) {
            service->Startup();
        }
    }

    void ServiceManager::RemoveService(std::shared_ptr<IService> service)
    {
        auto it = std::find(services.begin(), services.end(), service);
        if (it == services.end()) {
            // Silently ignore the fact that the service could not be found.
            return;
        }

        // Stop service.
        (**it).Shutdown();

        // Remove service from list.
        services.erase(it);
    }

    bool ServiceManager::HasService(std::shared_ptr<IService> service) const
    {
        return std::find(services.begin(), services.end(), service) != services.end();
    }

    void ServiceManager::StartupAll()
    {
        if (IsRunning()) {
            throw std::logic_error("Services already started");
        }

        // Start up service according to the order they have been added.
        for (auto it = services.begin(); it != services.end(); ++it) {
            IService & service = **it;
            service.Startup();
        }        
        running = true;
    }

    void ServiceManager::ShutdownAll()
    {
        if (!IsRunning()) {
            // Silently ignore the fact that no services are running anyway.
            return;
        }

        // Shut down service in reverse order.
        for (auto it = services.rbegin(); it != services.rend(); ++it) {
            IService & service = **it;
            service.Shutdown();
        }        
        running = false;
    }

    bool ServiceManager::IsRunning() const
    {
        return running;
    }


} // end of namespace