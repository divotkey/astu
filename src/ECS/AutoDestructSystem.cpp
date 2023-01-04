/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "ECS/AutoDestructSystem.h"
#include "ECS/CAutoDestruct.h"

namespace astu {

using namespace std;

    const astu::EntityFamily AutoDestructSystem::FAMILY 
                    = EntityFamily::Create<CAutoDestruct>();

    AutoDestructSystem::AutoDestructSystem(int updatePriority)
        : Service("Auto Destruct System")
        , IteratingEntitySystem(FAMILY, updatePriority)
    {
        // Intentionally left empty.
    }

    void AutoDestructSystem::OnStartup()
    {
        // Intentionally left empty.
    }

    void AutoDestructSystem::OnShutdown()
    {
        // Intentionally left empty.
    }

    void AutoDestructSystem::ProcessEntity(Entity& entity)
    {
        CAutoDestruct& ac = entity.GetComponent<CAutoDestruct>();

        ac.duration -= GetElapsedTimeF();
        if (ac.duration <= 0) {
            ASTU_SERVICE(EntityService).RemoveEntity(entity);
        }
    }

} // end of namespace