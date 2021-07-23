/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "EntityService.h"

// C++ Standard Libraries includes
#include <memory>
#include <string>
#include <map>

namespace astu {

    /**
     * This entity factory is used to generate new entities, where entities are
     * defined by prototypes that are cloned when new entities are created. 
     * 
     * @ingroup ecs_group
     */
    class EntityFactoryService : public virtual Service {
    public:

        /**
         * Constructor.
         */
        EntityFactoryService();
        
        /**
         * Tests whether a entity has been register.
         * 
         * @param protoName  the name under which the entity has been registered
         * @return `true` if an entity has been registered with the given name
         */
        bool HasPrototype(const std::string & protoName) const;

        /**
         * Registers a new entity prototype.
         * 
         * @param protoName the name of the prototype
         * @param proto     the entity prototype 
         * @throws std::logic_error in case the given name is already used
         */
        void RegisterPrototype(
            const std::string & protoName, std::shared_ptr<Entity> proto);

        /**
         * Deregisters an entityentity prototype.
         * 
         * @param protoName the name of the prototype
         */
        void DeregisterPrototype(const std::string & protoName);

        /**
         * Deregisters all prototypes.
         */
        void DeregisterAllPrototypes();

        /**
         * Creates a new entity based on a registered prototype.
         * 
         * @param protoName the name of the prototype
         * @throws std::logic_error in case the given name is unknown
         */
        std::shared_ptr<Entity> CreateEntity(const std::string & protoName) const;

    private:
        /** The registered prototypes. */
        std::map<std::string, std::shared_ptr<Entity>> prototypes;

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
    };

} // end of namespace

