/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Service.h"
#include "Service/TimeService.h"
#include "ECS/EntitySystems.h"

namespace astu {

    /**
     * This entity system removes entities after a certain amount of time.
     * 
     * This system relies on CPose and CAutoDestruct components.
     * 
     * @ingroup ecs_group
     */
    class AutoDestructSystem 
        : public BaseService
        , private IteratingEntitySystem
        , private TimeClient
    {
    public:

        /**
         * Constructor.
         * 
         * @param updatePriority    the priority used to update this system
         */        
        AutoDestructSystem(int updatePriority = astu::Priority::Normal);

    private:
        // My Entity family.
        static const astu::EntityFamily FAMILY;

        // Inherited via BaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via IteratingEntitySystem
        void ProcessEntity(astu::Entity& entity);
    };

} // end of namespace

