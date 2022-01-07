/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Service/SignalService.h"
#include "ECS/EntityService.h"

namespace astu::suite2d {
    
    /**
     * General interface to physics system in two-dimensional environments.
     * 
     * @ingroup suite2d_group
     */
    class PhysicsSystem  {
    public:

        /** Virtual destructor. */
        virtual ~PhysicsSystem()  {}

        /**
         * Sets the gravity vector for the physics simulation.
         * 
         * @param g the gravity vector
         * @return reference to this system for method chaining
         */
        PhysicsSystem& SetGravityVector(const Vector2f& g) {
            return SetGravityVector(g.x, g.y);
        }

        /**
         * Sets the gravity vector for the physics simulation.
         * 
         * @param gx    the x-component of the gravity vector
         * @param gy    the y-component of the gravity vector
         * @return reference to this system for method chaining
         */
        virtual PhysicsSystem& SetGravityVector(float gx, float gy) = 0;

        /**
         * Returns the gravity vector of the physics simulation.
         * 
         * @return the gravity vector
         */
        virtual const Vector2f& GetGravityVector() const = 0;
    };

} // end of namespace

