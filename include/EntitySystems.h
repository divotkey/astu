/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>

// Local includes
#include "Service.h"
#include "EntityService.h"

namespace astu {

    /**
     * A system which processes one family of entities.
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
         * Call `ProcessEntity` for all entities of the processed entity family.
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

    class EntityListener : public virtual Service, private IEntityListener {
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
