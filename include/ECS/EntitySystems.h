/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>

// Local includes
#include "EntityService.h"

namespace astu {

    /**
     * A system which processes one family of entities.
     * 
     * **Example One-Family Entity-System**
     * 
     * This example shows how to build your own entity system that uses
     * exactly one family of entities
     * 
     * Header file
     * 
     * @include MyOnyFamilySystem.h
     * 
     * Implementation file
     * @include MyOnyFamilySystem.cpp
     * 
     * @ingroup ecs_group
     */
    class OneFamilyEntitySystem : public virtual Service {
    public:

        /**
         * Constructor.
         * 
         * @param family    the family of entities this system processes
         */
        OneFamilyEntitySystem(const EntityFamily & family) {

            AddStartupHook([this, family](){ 
                entityService = ASTU_GET_SERVICE(EntityService);
                entityView = entityService->GetEntityView(family); 
            });

            AddShutdownHook([this](){ 
                entityView = nullptr; entityService = nullptr;
            });
        }
    
        /**
         * Returns the view to the family of entities this system processes.
         * 
         * @return the entity view
         */
        EntityView& GetEntityView() {
            return *entityView;
        }

        /**
         * Returns the view to the family of entities this system processes.
         * 
         * @return the entity view
         */
        const EntityView& GetEntityView() const {
            return *entityView;
        }

    protected:

        /**
         * CallFunction `ProcessEntity` for all entities of the processed entity family.
         */
        void ProcessEntities() {
            for (const auto & entity : *entityView) {
                ProcessEntity(*entity);
            }
        }

        /**
         * Called by 'ProcessEntities'.
         * 
         * @param entity    the entity to process
         */
        virtual void ProcessEntity(astu::Entity & entity) {}

        /**
         * Convenient method to access the entity service.
         * 
         * @return the entity service
         */
        EntityService& GetEntityService() {
            return *entityService;
        }

    private:
        /** The view to the family of entities. */
        std::shared_ptr<EntityView> entityView;

        /** Used to access the entity service. */
        std::shared_ptr<EntityService> entityService;
    };

    /**
     * A system which iterates over a family of entities.
     * 
     * **Example Iterating Entity-System**
     * 
     * This example shows how to build your own entity system that iterates
     * over a single family of entities. 
     * 
     * Header file
     * 
     * @include MyIteratingEntitySystem.h
     * 
     * Implementation file
     * @include MyIteratingEntitySystem.cpp
     * 
     * @ingroup ecs_group
     */
    class IteratingEntitySystem 
        : public virtual Service
        , private IUpdatable
    {
    public:

        /**
         * Constructor.
         * 
         * @param family    the family of entities this system processes
         * @param priority  the update priority
         */
        IteratingEntitySystem(
            const EntityFamily& family, 
            int priority = Priority::Normal)
            : updatePriority(priority)
        {

            AddStartupHook([this, family](){ 
                entityService = ASTU_GET_SERVICE(EntityService);
                entityView = entityService->GetEntityView(family); 
            });

            AddShutdownHook([this](){ 
                entityView = nullptr; entityService = nullptr;
            });

            AddStartupHook([this, priority]() { 
                ASTU_SERVICE(UpdateService).AddUpdatable(*this, priority); } );

            AddShutdownHook([this]() { 
                ASTU_SERVICE(UpdateService).RemoveUpdatable(*this); } );

            AddPauseHook([this]() { 
                if (GetStatus() == Running) {
                    ASTU_SERVICE(UpdateService).RemoveUpdatable(*this);
                }            
            });

            AddResumeHook([this]() { 
                if (GetStatus() == Paused) {
                    ASTU_SERVICE(UpdateService).AddUpdatable(*this, updatePriority);
                }
            });
        }
    
    protected:

        /**
         * Convenient method to access the entity service.
         * 
         * @return the entity service
         */
        EntityService& GetEntityService() {
            return *entityService;
        }

        /**
         * Returns the view to the family of entities this system processes.
         * 
         * @return the entity view
         */
        const EntityView& GetEntityView() const {
            return *entityView;
        }

        /**
         * Called by 'ProcessEntities'.
         * 
         * @param entity    the entity to process
         */
        virtual void ProcessEntity(astu::Entity & entity) {}

        // Inherited via IUpdatable
        virtual void OnUpdate() override {
            
            // Iterate over all entities of out family.
            for (auto &entity : *entityView) {
                ProcessEntity(*entity);
            }
        }

    private:
        /** The view to the family of entities. */
        std::shared_ptr<EntityView> entityView;

        /** Used to access the entity service. */
        std::shared_ptr<EntityService> entityService;

        /** The update priority of this service. */
        int updatePriority;
    };

    /**
     * Services can derive from this class to get informed when an entity
     * of a certain family is added or remove to the entity service.
     * 
     * @ingroup ecs_group
     */
    class EntityListener 
        : public virtual Service 
        , private IEntityListener
    {
    public:

        /**
         * Constructor.
         * 
         * @param family    the family of entities this system processes
         */
        EntityListener(const EntityFamily & family) {

            AddStartupHook([this, family](){ 
                ASTU_SERVICE(EntityService).AddEntityListener(family, *this); 
            });


            AddShutdownHook([this, family](){ 
                ASTU_SERVICE(EntityService).RemoveEntityListener(family, *this); 
            });
        }

        // Inherited via IEntityListener
        virtual void OnEntityAdded(std::shared_ptr<astu::Entity> entity) override {}
        virtual void OnEntityRemoved(std::shared_ptr<astu::Entity> entity) override {}
    };

} // end of namespace
