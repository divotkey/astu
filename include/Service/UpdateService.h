/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <vector>

// Local includes.
#include "Service.h"
#include "Priority.h"
#include "Util/ListenerManager.h"

namespace astu {

    /**
     * Interface for items that can be updated.
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
         * Called when an update is due.
         */
        virtual void OnUpdate() = 0;
    };

    /**
     * The update service updates registered `Updatables` and maintains the update
     * priority.
     * 
     * To update the `Updatables`, this service's method
     * `UpdateAll` must be called within the simulation 
     * respectively game loop.
     * 
     * @ingroup srv_group
     */
    class UpdateService final : public Service {
    public:

        /**
         * Constructor.
         */
        UpdateService();

        /**
         * Adds an updatable.
         * 
         * @param updatable the updatable to be added
         * @param priority  the update priority to be used
         */        
        void AddUpdatable(IUpdatable & updatable, int priority = Priority::Normal);

        /**
         * Removes an updatable.
         * 
         * @param updatable the updatable to be removed
         */
        void RemoveUpdatable(IUpdatable & updatable);

        /**
         * Tests whether a specific updatable has already been added.
         * 
         * @param updatable the updatable to be tested
         * @return `true` if the updatable has already been added
         */
        bool HasUpdatable(IUpdatable & updatable) const;

        /**
         * Returns the update priority of an updatable.
         * 
         * @param updatable which updater priority should be returned
         * @throws std::logic_error in case the updatable is unknown
         */
        int GetUpdatePriority(IUpdatable & updatable) const;

        /**
         * Updates all registered updatables.
         */
        void UpdateAll();

    private:
        /** Used to organize updatables. */
        SortingRawListenerManager<IUpdatable> lstMngr;
    };

    /**
     * Inherit from this class to get updated as a service.
     */
    class Updatable : public virtual Service, private IUpdatable {
    public:

        /**
         * Constructor.
         * 
         * @param priority  the update priority
         */
        Updatable(int priority = Priority::Normal);

        /** Virtual destructor. */
        virtual ~Updatable() {}

    protected:

        // Inherited via IUpdatable
        virtual void OnUpdate() override {}

    private:
        /** The update priority of this updatable. */
        int updatePriority;
    }; 

} // end of namespace