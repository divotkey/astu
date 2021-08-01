/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "ECS/EntityService.h"

// C++ Standard Library includes
#include <memory>

namespace astu::suite2d {

    /**
     * Base class for physics based point masses in two-dimensional worlds.
     * 
     * @ingroup suite2d_group
     */
    class CBody : public astu::EntityComponent {
    public: 

        /**
         * Enumeration describing the type of physics bodies.
         */
        enum Type {
            /** Static bodies that never move. */
            Static, 

            /** Kinematic bodies can be moved by the game logic. */
            Kinematic, 

            /** Dynamic bodies get moved by the physics system. */
            Dynamic
        };

        /**
         * Constructor.
         */
        CBody() 
            : type(Type::Static)
            , linearVelocity(0, 0)
            , angularVelocity(0)
            , linearDamping(0)
            , angularDamping(0)
        {
            // Intentionally left empty.        
        }

        /**
         * Returns the type of this body.
         * 
         * @return the type of this body
         */
        Type GetType() const {
            return type;
        }

        /**
         * Sets the type of this body.
         * 
         * @param bodyType the type of this body
         */
        virtual void SetType(Type bodyType) {
            type = bodyType;
        }

        /**
         * Sets the linear velocity of this body.
         * 
         * @param v the linear velocity
         */
        virtual void SetLinearVelocity(const Vector2f& v) {
            SetLinearVelocity(v.x, v.y);
        }

        /**
         * Sets the linear velocity of this body.
         * 
         * @param vx    the x-component of the linear velocity
         * @param vy    the y-component of the linear velocity
         * @return reference to this body for method chaining
         */
        virtual CBody& SetLinearVelocity(float vx, float vy) {
            linearVelocity.Set(vx, vy);
            return *this;
        }

        /**
         * Returns the linear velocity at the center of mass.
         * 
         * @return the linear velocity
         */
        virtual Vector2f GetLinearVelocity() const {
            return linearVelocity;
        }

        /**
         * Sets the angular velocity.
         * 
         * @param av    the angular velocity in radians per second
         */
        virtual CBody& SetAngularVelocity(float av) {
            angularVelocity = av;
            return *this;
        }

        /**
         * Returns the angular velocity of this body.
         * 
         * @return the angular velocity
         */
        virtual float GetAngularVelocity() const {
            return angularVelocity;
        }

        /**
         * Returns the linear damping of this body.
         * 
         * @return the linear damping
         */
        float GetLinearDamping() const {
            return linearDamping;
        }

        /**
         * Sets the linear damping of this body.
         * 
         * @param damping   the damping for linear movement
         */
        virtual void SetLinearDamping(float damping) {
            linearDamping = linearDamping;
        }

        /**
         * Returns the angular damping of this body.
         * 
         * @return the angular damping
         */
        float GetAngularDamping() const {
            return angularDamping;
        }

        /**
         * Sets the angular damping of this body.
         * 
         * @param damping   the damping for rotational movement
         */
        virtual void SetAngularDamping(float damping) {
            angularDamping = angularDamping;
        }


    private:
        /** The type of this body. */
        Type type;

        /** The linear velocity of this body. */
        Vector2f linearVelocity;

        /** The angular velocity of this body. */
        float angularVelocity;

        /** The damping for linear movement. */
        float linearDamping;

        /** The damping for rotational movement. */
        float angularDamping;
    };


    /**
     * Abstract factory for for CBody components.
     * 
     * @ingroup suite2d_group
     */
    class CBodyFactory {
    public:

        /** Virtual destructor. */
        virtual ~CBodyFactory() {}

        /**
         * Creates a new CBody instance.
         */
        virtual std::shared_ptr<CBody> CreateBody() = 0;
    };


    /**
     * Builder for CBody components.
     * 
     * @ingroup suite2d_group
     */
    class CBodyBuilder final {
    public:

        /**
         * Constructor.
         * 
         * If the specified body factory is null a service which implements
         * the body factory interface will be used.
         * 
         * @param bodyFactory   the factory for CBody components to be used
         */
        CBodyBuilder(std::shared_ptr<CBodyFactory> bodyFactory = nullptr);

        /**
         * Sets the type of the body to create.
         * 
         * @param bodyType  the type of the body
         * @return reference to this builder for method chaining
         */
        CBodyBuilder& Type(CBody::Type bodyType) {
            type = bodyType;
            return *this;
        }

        /**
         * Sets the linear velocity of the body to create body.
         * 
         * @param vx    the x-component of the linear velocity
         * @param vy    the y-component of the linear velocity
         * @return reference to this builder for method chaining
         */
        CBodyBuilder& LinearVelocity(float vx, float vy) {
            linearVelocity.Set(vx, vy);
            return *this;
        }

        /**
         * Sets the linear velocity of the body to create body.
         * 
         * @param v the velocity vector
         * @return reference to this builder for method chaining
         */
        CBodyBuilder& LinearVelocity(const Vector2f& v) {
            linearVelocity.Set(v);
            return *this;    
        }

        /**
         * Sets the angular velocity of the body to create body.
         * 
         * @param av    the angular velocity in radians per second
         * @return reference to this builder for method chaining
         */
        CBodyBuilder& AngularVelocity(float av) {
            angularVelocity = av;
            return *this;
        }        

        /**
         * Sets the angular damping of the body to create
         * 
         * @param damping   the damping for rotational movement
         * @return reference to this builder for method chaining
         * @throws std::domain_error in case the damping is less than zero
         */
        CBodyBuilder& AngularDamping(float damping) {
            if (damping < 0) {
                throw std::domain_error(
                    "Angular damping must be greater or equal zero");
            }
            angularDamping = damping;
            return *this;
        }

        /**
         * Sets the linear damping of the body to create
         * 
         * @param damping   the damping for linear movement
         * @return reference to this builder for method chaining
         * @throws std::domain_error in case the damping is less than zero
         */
        CBodyBuilder& LinearDamping(float damping) {
            if (damping < 0) {
                throw std::domain_error(
                    "Linear damping must be greater or equal zero");
            }
            linearDamping = damping;
            return *this;
        }

        /**
         * Resets this build to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        CBodyBuilder& Reset() {
            type = CBody::Type::Static;
            linearVelocity.SetZero();
            angularVelocity = 0;
            angularDamping = 0;
            linearDamping = 0;
            return *this;
        }

        /**
         * Creates a new CBody instance according to the current configuration.
         * 
         * @return the new CBody instance
         */
        std::shared_ptr<CBody> Build();
        
    private:
        /** The factory used to create the CBody instances. */
        std::shared_ptr<CBodyFactory> bodyFactory;

        /** The type of the CBody to create. */
        CBody::Type type;

        /** The linear velocity of the CBody to create. */
        Vector2f linearVelocity;

        /** The angular velocity of the CBody to create. */
        float angularVelocity;

        /** The linear damping of the CBody to create. */
        float linearDamping;

        /** The angular damping of the CBody to create. */
        float angularDamping;
    };

} // end of namespace