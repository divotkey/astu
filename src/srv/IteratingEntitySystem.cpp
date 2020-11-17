/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "IteratingEntitySystem.h"

namespace astu {

    IteratingEntitySystem::IteratingEntitySystem(const EntityFamily & itFamily, int priority)
        : BaseService("Iterating Entity")
        , updatePriority(priority)
        , iterateFamily(itFamily)
    {
        // Intentioniall left empty.
    }

    void IteratingEntitySystem::OnStartup() 
    {
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

    void IteratingEntitySystem::OnShutdown() 
    {
        // Cleanup
        timeService = nullptr;
        entityView = nullptr;
        GetSM().GetService<EntityService>().RemoveEntityListener(iterateFamily, shared_from_this());
        GetSM().GetService<UpdateService>().RemoveUpdatable(shared_from_this());
    }

    void IteratingEntitySystem::OnUpdate()
    {
        for (auto & entity : *entityView) {
            ProcessEntity(*entity);
        }
    }

} // end of namespace