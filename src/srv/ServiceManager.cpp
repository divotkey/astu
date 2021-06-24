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
#include <algorithm>


using namespace std;

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

    void ServiceManager::AddService(shared_ptr<Service> service)
    {
        if (HasService(service)) {
            throw logic_error("Service '" + service->GetName() 
                + "' has already been added");
        }

        services.push_back(service);

        // Start service if all other service are currently running.
        if (running) {
            service->Startup();
        }
    }

    void ServiceManager::RemoveService(shared_ptr<Service> service)
    {
        auto it = find(services.begin(), services.end(), service);
        if (it == services.end()) {
            // Silently ignore the fact that the service could not be found.
            return;
        }

        // Stop service.
        (**it).Shutdown();

        // Remove service from list.
        services.erase(it);
    }

    bool ServiceManager::HasService(shared_ptr<Service> service) const
    {
        return find(services.begin(), services.end(), service) != services.end();
    }

    void ServiceManager::StartupAll()
    {
        if (IsRunning()) {
            throw logic_error("Services already started");
        }

        // Start up service according to the order they have been added.
        for (auto it = services.begin(); it != services.end(); ++it) {
            Service & service = **it;
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
            Service & service = **it;
            service.Shutdown();
        }        
        running = false;
    }

    bool ServiceManager::IsRunning() const
    {
        return running;
    }

} // end of namespace