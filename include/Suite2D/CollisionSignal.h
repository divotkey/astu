/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/SignalService.h"
#include "ECS/EntityService.h"

namespace astu::suite2d {

    /**
     * This signal represents a collision between two entities.
     * 
     * @ingroup suite2d_group
     */
    class CollisionSignal {
    public:

        /**
         * Constructor.
         * 
         * @param a the first entity
         * @param b the second entity
         */
        CollisionSignal(
            std::shared_ptr<astu::Entity> a,
            std::shared_ptr<astu::Entity> b
        )
            : entityA(a), entityB(b)
        {
            // Intentionally left empty
        }

        /** The first entity involved in the collision. */
        std::shared_ptr<astu::Entity> entityA;

        /** The second entity involved in the collision. */
        std::shared_ptr<astu::Entity> entityB;
    };

    /** 
     * Type definition for signal services that transmit collision signals.
     *
     * @ingroup suite2d_group
     */
    using CollisionSignalService = SignalService<CollisionSignal>;

    /** 
     * Type definition for signal listeners which receive collision signals.
     *
     * @ingroup suite2d_group
     */
    using ICollisionListener = ISignalListener<CollisionSignal>;

    /**
     * Services can derive from this class to process mouse wheel signals.
     * 
     * @ingroup suite2d_group
     */
    class CollisionListener 
        : virtual public Service
        , private ICollisionListener
    {
    public:

        /**
         * Constructor.
         */
        CollisionListener() {
            AddStartupHook([this](){ 
                    ASTU_SERVICE(CollisionSignalService).AddListener(*this); 
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(CollisionSignalService).RemoveListener(*this); 
            });
        }

    protected:

        /** 
         * Called by this base class when a collision event has been received
         * 
         * @param entityA   the first entity
         * @param entityB   the second entity
         */
        virtual bool OnCollision(astu::Entity& entityA, astu::Entity& entityB) { 
            return false;
        }

    private:

        // Inherited via ICollisionListener 
        virtual bool OnSignal(const CollisionSignal & signal) {
            return OnCollision(*signal.entityA, *signal.entityB);
        }
    };


} // end of namespace