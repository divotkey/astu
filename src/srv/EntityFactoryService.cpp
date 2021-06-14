/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Libraries includes
#include <stdexcept>

// Local includes
#include "EntityFactoryService.h"

using namespace std;

namespace astu {

    EntityFactoryService::EntityFactoryService()
        : Service("Entity Factory Service")
    {
        // Intentionally left empty.
    }

    void EntityFactoryService::OnStartup()
    {
        // Intentionally left empty.
    }

    void EntityFactoryService::OnShutdown()
    {
        prototypes.clear();
    }

    bool EntityFactoryService::HasPrototype(const string & name) const 
    {
        return prototypes.find(name) != prototypes.end();
    }

    void EntityFactoryService::RegisterPrototype(
        const string & name, shared_ptr<Entity> proto)
    {
        if (HasPrototype(name)) {
            throw logic_error("Unable to register prototype, ambiguous name '" 
                + name + "'");
        }        

        prototypes[name] = proto;
    }

    void EntityFactoryService::DeregisterPrototype(const std::string & name)
    {
        auto it = prototypes.find(name);
        if (it != prototypes.end()) {
            prototypes.erase(it);
        }
    }

    void EntityFactoryService::DeregisterAllPrototypes()
    {
        prototypes.clear();
    }

    shared_ptr<Entity> EntityFactoryService::CreateEntity(const string & name) const
    {
        auto it = prototypes.find(name);
        if (it == prototypes.end()) {
            throw logic_error("Unable to create entity, ptototype '" 
                + name + "' is unknown");
        }

        return it->second->Clone();
    }

} // end of namespace