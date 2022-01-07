/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "ECS/EntityService.h"

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

    /**
     * Services can derive from this class to have easily create and add entities.
     * 
     * @ingroup ecs_group
     */
    class EntityFactoryClient : virtual Service {
    public:

        /**
         * Constructor.
         */
        EntityFactoryClient() {
            AddStartupHook([this]() { 
                factoryService = ASTU_GET_SERVICE(EntityFactoryService);
                entityService = ASTU_GET_SERVICE(EntityService);
            });

            AddShutdownHook([this]() {
                factoryService = nullptr;
                entityService = nullptr;
            });
        }

        /** Virtual destructor. */
        virtual ~EntityFactoryClient() { }

    protected:

        /**
         * Creates a new entity based on the specified prototype.
         * 
         * @param protoName the name of the prototype
         * @return the newly created entity
         * @throws std::logic_error in case the prototype is unknown
         */
        std::shared_ptr<Entity> CreateEntity(const std::string & protoName)
        {
            return factoryService->CreateEntity(protoName);
        }

        /**
         * Creates a new entity based on the specified prototype.
         * 
         * This method requires the entity to contain a astu2d::CPose component.
         * 
         * @param protoName the name of the prototype
         * @param posX      the x-coordinate of the position of the entity
         * @param posY      the y-coordinate of the position of the entity
         * @param phi       the angle of the entity in radians
         * @return the newly created entity
         * @throws  std::logic_error in case the prototype is unknown or if the
         *          entity does not contain a CPose component
         */
        std::shared_ptr<Entity> CreateEntity(
            const std::string & protoName, 
            float posX, 
            float posY, 
            float phi = 0);

        /**
         * Creates a new entity based on the specified prototype.
         * 
         * This method requires the entity to contain a astu2d::CPose component.
         * 
         * @param protoName the name of the prototype
         * @param pos       the position of the entity
         * @param phi       the angle of the entity in radians
         * @return the newly created entity
         * @throws  std::logic_error in case the prototype is unknown or if the
         *          entity does not contain a CPose component
         */
        std::shared_ptr<Entity> CreateEntity(
            const std::string & protoName, 
            const Vector2f& pos, 
            float phi = 0)
        {
            return CreateEntity(protoName, pos.x, pos.y, phi);            
        }

        /**
         * Creates and adds a new entity based on the specified prototype.
         * 
         * @param protoName the name of the prototype
         * @return the newly created entity
         * @throws std::logic_error in case the prototype is unknown
         */
        std::shared_ptr<Entity> AddEntity(const std::string & protoName) {
            auto entity = CreateEntity(protoName);
            entityService->AddEntity(entity);
            return entity;
        }

        /**
         * Creates and adds a new entity based on the specified prototype.
         * 
         * This method requires the entity to contain a astu2d::CPose component.
         * 
         * @param protoName the name of the prototype
         * @param pos       the position of the entity
         * @param phi       the angle of the entity in radians
         * @return the newly created entity
         * @throws  std::logic_error in case the prototype is unknown or if the
         *          entity does not contain a CPose component
         */
        std::shared_ptr<Entity> AddEntity(
            const std::string & protoName, 
            float posX, 
            float posY, 
            float phi = 0)
        {
            auto entity = CreateEntity(protoName, posX, posY, phi);
            entityService->AddEntity(entity);
            return entity;
        }

        /**
         * Creates and adds a new entity based on the specified prototype.
         * 
         * This method requires the entity to contain a astu2d::CPose component.
         * 
         * @param protoName the name of the prototype
         * @param posX      the x-coordinate of the position of the entity
         * @param posY      the y-coordinate of the position of the entity
         * @param phi       the angle of the entity in radians
         * @return the newly created entity
         * @throws  std::logic_error in case the prototype is unknown or if the
         *          entity does not contain a CPose component
         */
        std::shared_ptr<Entity> AddEntity(
            const std::string & protoName, 
            const Vector2f& pos, 
            float phi = 0)
        {
            return AddEntity(protoName, pos.x, pos.y, phi);
        }

    private:
        /** The entity service used to add entities. */
        std::shared_ptr<EntityService> entityService;

        /** The entity factory service used. */
        std::shared_ptr<EntityFactoryService> factoryService;
    };

} // end of namespace

