/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
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
        // Register as updatable. 
        GetSM().GetService<UpdateService>().AddUpdatable(shared_as<IteratingEntitySystem>());

        // Get view to entites.
        auto & es = GetSM().GetService<EntityService>();
        es.AddEntityListener(iterateFamily, shared_as<IteratingEntitySystem>());
        entityView = es.GetEntityView(iterateFamily);

        // Get pointer to time service.
        timeService = GetSM().FindService<ITimeService>();
        if (!timeService) {
            throw std::logic_error("Iterating entity system service requires time service");
        }

        // Don't forget to call startup on base service!
        BaseService::Startup();
    }

    void IteratingEntitySystem::Shutdown() 
    {
        // Don't forget to call shutdown on base service!
        BaseService::Shutdown();

        // Cleanup
        timeService = nullptr;
        entityView = nullptr;
        GetSM().GetService<EntityService>().RemoveEntityListener(iterateFamily, shared_as<IteratingEntitySystem>());
        GetSM().GetService<UpdateService>().RemoveUpdatable(shared_as<IteratingEntitySystem>());
    }

    void IteratingEntitySystem::OnUpdate()
    {
        for (auto & entity : *entityView) {
            ProcessEntity(*entity);
        }
    }

    int IteratingEntitySystem::GetUpdatePriority() const
    {
        return updatePriority;
    }

} // end of namespace