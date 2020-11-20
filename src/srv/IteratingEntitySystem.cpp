/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "IteratingEntitySystem.h"

namespace astu {

    IteratingEntitySystem::IteratingEntitySystem(const EntityFamily & itFamily, int priority, const std::string & name)
        : BaseService(name)
        , updatePriority(priority)
        , iterateFamily(itFamily)
    {
        // Intentioniall left empty.
    }

    void IteratingEntitySystem::Startup() 
    {
        // Don't forget to call startup on base service!
        BaseService::Startup();

        // Register as updatable. 
        GetSM().GetService<UpdateService>().AddUpdatable(shared_from_this());

        // Get view to entites.
        auto & es = GetSM().GetService<EntityService>();
        es.AddEntityListener(iterateFamily, shared_from_this());
        entityView = es.GetEntityView(iterateFamily);

        // Get pointer to time service.
        timeService = GetSM().FindService<ITimeService>();
        if (!timeService) {
            throw std::logic_error("Iterating entity system service requires time service");
        }
    }

    void IteratingEntitySystem::Shutdown() 
    {
        // Cleanup
        timeService = nullptr;
        entityView = nullptr;
        GetSM().GetService<EntityService>().RemoveEntityListener(iterateFamily, shared_from_this());
        GetSM().GetService<UpdateService>().RemoveUpdatable(shared_from_this());

        // Don't forget to call shutdown on base service!
        BaseService::Shutdown();
    }

    void IteratingEntitySystem::OnUpdate()
    {
        for (auto & entity : *entityView) {
            ProcessEntity(*entity);
        }
    }

} // end of namespace