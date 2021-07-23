/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/UpdateService.h"
#include "ECS/EntitySystems.h"
#include "Scene.h"

namespace astu::suite2d {

    /**
     * A component system that renders Spatial components using the 2D scene
     * graph facility. 
     * 
     * @ingroup suite2d_group
     */
    class SceneSystem 
        : public Updatable
        , private OneFamilyEntitySystem
        , private EntityListener
    {
    public:

        /**
         * Constructor.
         * 
         * @param updatePriority    the priority used to update this system
         */
        SceneSystem(int updatePriority = 0);

    private:
        /** The entity family this system processes. */
        static const astu::EntityFamily FAMILY;

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via Updatable
        virtual void OnUpdate() override;    

        // Inherited via OneFamilyEntitySystem
        virtual void ProcessEntity(astu::Entity & entity) override;

        // Inherited via EntityListener
        virtual void OnEntityAdded(std::shared_ptr<astu::Entity> entity) override;
        virtual void OnEntityRemoved(std::shared_ptr<astu::Entity> entity) override;

        /** The root node where to attach the entities. */
        std::shared_ptr<Node> root;
    };

} // end of namespace