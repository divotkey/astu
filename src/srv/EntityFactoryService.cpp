/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "EntityFactoryService.h"

// C++ Standard Libraries includes
#include <stdexcept>

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

    bool EntityFactoryService::HasPrototype(const string & protoName) const 
    {
        return prototypes.find(protoName) != prototypes.end();
    }

    void EntityFactoryService::RegisterPrototype(
        const string & protoName, shared_ptr<Entity> proto)
    {
        if (HasPrototype(protoName)) {
            throw logic_error("Unable to register prototype, ambiguous name '" 
                + protoName + "'");
        }        

        prototypes[protoName] = proto;
    }

    void EntityFactoryService::DeregisterPrototype(const std::string & protoName)
    {
        auto it = prototypes.find(protoName);
        if (it != prototypes.end()) {
            prototypes.erase(it);
        }
    }

    void EntityFactoryService::DeregisterAllPrototypes()
    {
        prototypes.clear();
    }

    shared_ptr<Entity> EntityFactoryService::CreateEntity(const string & protoName) const
    {
        auto it = prototypes.find(protoName);
        if (it == prototypes.end()) {
            throw logic_error("Unable to create entity, ptototype '" 
                + protoName + "' is unknown");
        }

        return it->second->Clone();
    }

} // end of namespace