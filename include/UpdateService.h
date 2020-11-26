/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <vector>
#include <memory>
#include "Service.h"
#include "ListenerManager.h"

namespace astu {

    /**
     * Interface for anything which is updatable and should be managed by the `UpdateService`.
     * 
     * @ingroup srv_group
     */
    class IUpdatable {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~IUpdatable() {}

        /**
         * Called when this updatable gets updated.
         */
        virtual void OnUpdate() = 0;

        /**
         * Returns the update priority.
         *  
         * Updatables with lower priorities get updated before updatables with higher priorities.
         * 
         * @return the update priority
         */
        virtual int GetUpdatePriority() const = 0;

    };

    /**
     * This service manages and updates `Updatables`.
     * 
     * To update the `Updatables`, this service's method
     * `UpdateAll` must be called within the simulation 
     * respectively game loop.
     * 
     * @ingroup srv_group
     */
    class UpdateService : public BaseService {
    public:

        /**
         * Constructor.
         */
        UpdateService();

        /**
         * Adds an updatable.
         * 
         * @param updatable the updatable to be added
         */        
        void AddUpdatable(std::shared_ptr<IUpdatable> updatable);

        /**
         * Removes an updatable.
         * 
         * @param updatable the updatable to be removed
         */
        void RemoveUpdatable(std::shared_ptr<IUpdatable> updatable);

        /**
         * Tests whether a specific updatablel has already been added.
         * 
         * @param updatable the updatable to be tested
         * @return `true` if the updatale has already beed added
         */
        bool HasUpdatable(std::shared_ptr<IUpdatable> updatable);

        /***
         * Updates all registered updatables.
         */
        void UpdateAll();

    private:
        /** Used to organize updatables. */
        SortingListenerManager<IUpdatable> lstMngr;
    };

    /**
     * Base class for services whitch require an update.
     * 
     * @ingroup srv_group
     */
    class UpdatableBaseService 
        : public BaseService
        , public IUpdatable
    {
    public:

        /**
         * Constructor.
         * 
         * @param name      the name of this service
         * @param priority  the update priority of this service
         */
        UpdatableBaseService(const std::string & name = DEFAULT_NAME, int priority = 0);

        // Inherited via BaseService/IService
        virtual void Startup() override;
        virtual void Shutdown() override;

        // Inherited via IUpdatable.
        virtual int GetUpdatePriority() const override;

    private:
        /** The update priority of this updatable. */
        int updatePriority;
    };

} 