/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "ECS/EntityFactoryService.h"
#include "Suite2D/CPose.h"

// C++ Standard Libraries includes
#include <stdexcept>

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// EntityFactoryService
    /////////////////////////////////////////////////

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

    void EntityFactoryService::DeregisterPrototype(const string & protoName)
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

    /////////////////////////////////////////////////
    /////// EntityFactoryClient
    /////////////////////////////////////////////////

    shared_ptr<Entity> EntityFactoryClient::CreateEntity(const string & protoName, float posX, float posY, float phi)
    {
        auto entity = factoryService->CreateEntity(protoName);
        entity->GetComponent<astu::suite2d::CPose>()
            .transform.SetTranslation(posX, posY)
            .SetRotation(phi);

        return entity;
    }

} // end of namespace