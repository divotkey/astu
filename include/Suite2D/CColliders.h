/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "ECS/EntityService.h"
#include "Math/Polygon.h"
#include "Math/Vector2.h"

// C++ Standard Library includes
#include <stdexcept>
#include <cstdint>
#include <memory>

namespace astu::suite2d {

    /////////////////////////////////////////////////
    /////// CBodyCollider
    /////////////////////////////////////////////////

    /**
     * Base class for physics collider in two-dimensional worlds.
     * 
     * @ingroup suite2d_group
     */
    class CBodyCollider : public EntityComponent {
    public: 

        /**
         * Constructor.
         */
        CBodyCollider() 
            : restitution(0.5f)
            , friction(0.2f)
            , density(1.0f)
            , categoryBits(0x0001)
            , maskBits(0xffff)
            , offset(0, 0)
        {
            // Intentionally left empty.        
        }

        /**
         * Returns the coefficient of restitution.
         * 
         * @return the coefficient of restitution
         */
        float GetRestitution() const {
            return restitution;
        }

        /**
         * Sets the coefficient of restitution.
         * 
         * @param r the coefficient of restitution
         * @throws std::domain_error in case the coefficient is less than zero
         */
        virtual void SetRestitution(float r) {
            if (r < 0) {
                throw std::domain_error(
                    "Coefficient of restitution must be greater or equal zero");
            }
            restitution = r;
        }

        /**
         * Returns the friction coefficient.
         * 
         * @return the friction coefficient
         */
        float GetFriction() const {
            return friction;
        }
        
        /**
         * Sets the friction coefficient.
         * 
         * @param f the friction coefficient
         * @throws std::domain_error in case the coefficient is less than zero
         */
        virtual void SetFriction(float f) {
            if (f < 0) {
                throw std::domain_error(
                    "Friction coefficient must be greater or equal zero");
            }
            friction = f;
        }

        /**
         * Returns the density of this collider.
         * 
         * @return the density, usually in kg/m^2
         */
        float GetDensity() const {
            return density;
        }

        /**
         * Sets the density of this collider.
         * 
         * @param d the density, usually in kg/m^2
         * @throws std::domain_error in case the density is less than zero
         */
        virtual void SetDensity(float d) {
            if (d < 0) {
                throw std::domain_error(
                    "Density must be greater or equal zero");
            }
            density = d;
        }

        /**
         * Returns the category bits used for collision filtering.
         * 
         * @return the category bits
         */
        uint16_t GetCategoryBits() const {
            return categoryBits;
        }

        /**
         * Sets the category bits used for collision filtering.
         * 
         * @param bits  the category bits
         */
        virtual void SetCategoryBits(uint16_t bits) {
            categoryBits = bits;
        }

        /**
         * Returns the mask bits used for collision filtering.
         * 
         * This bitfield mask the categories that this collider does accept 
         * for collision.
         * 
         * @return the category bits
         */
        uint16_t GetMaskBits() const {
            return maskBits;
        }

        /**
         * Sets the mask bits used for collision filtering.
         * 
         * This bitfield mask the categories that this collider does accept 
         * for collision.
         * 
         * @param bits  the mask bits
         */
        virtual void SetMaskBits(uint16_t bits) {
            maskBits = bits;
        }

        /**
         * Returns the offset of this colider.
         * 
         * @return the offset relativ to the origin of its entity
         */
        const Vector2f& GetOffset() const {
            return offset;
        }

        /**
         * Sets the offset of this collider.
         * 
         * @param o the offset relativ to the origin of its entity
         */
        virtual void SetOffset(const Vector2f& o) {
            offset = o;
        }        

    private:
        /** The coefficient of restitution. */
        float restitution;

        /** The friction coefficient, usually in the range [0,1]. */
        float friction;

	    /** The density, usually in kg/m^2. */
    	float density;

        /** The collision category bits */
        uint16_t categoryBits;

        /** The collision mask bits. */
        uint16_t maskBits;

        /** The offset of this collider relative to the origin of its entity. */
        Vector2f offset;
    };


    /////////////////////////////////////////////////
    /////// CBodyColliderBuilder
    /////////////////////////////////////////////////

    /**
     * Base class for collider builders.
     * 
     * @ingroup suite2d_group
     */
    template <typename T>
    class CBodyColliderBuilder {
    public:

        /** Virtual destructor. */
        ~CBodyColliderBuilder() {}

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        T& Reset()
        {
            restitution = 0.5f;
            friction = 0.2f;
            density = 1.0f;
            categoryBits = 0x0001;
            maskBits = 0xffff;
            offset.SetZero();

            return static_cast<T&>(*this);
        }

        /**
         * Sets the the coefficient of restitution of the collider to build
         * 
         * @param r the the coefficient of restitution
         * @throws std::domain_error in case the coefficient is less than zero
         * @return reference to this builder for method chaining
         */
        T& Restitution(float r) {
            if (r < 0) {
                throw std::domain_error(
                    "Coefficient of restitution must be greater zero");
            }
            restitution = r;
            return static_cast<T&>(*this);
        }

        /**
         * Sets the friction coefficient of the collider to build.
         * 
         * @param f the friction coefficient
         * @throws std::domain_error in case the coefficient is less than zero
         * @return reference to this builder for method chaining
         */
        T& Friction(float f) {
            if (f < 0) {
                throw std::domain_error(
                    "Friction coefficient must be greater or equal zero");
            }
            friction = f;
            return static_cast<T&>(*this);
        }

        /**
         * Sets the density of the collider to build.
         * 
         * @param d the density, usually in kg/m^2
         * @throws std::domain_error in case the density is less than zero
         * @return reference to this builder for method chaining
         */
        T& Density(float d) {
            if (d < 0) {
                throw std::domain_error(
                    "Density must be greater or equal zero");
            }
            density = d;
            return static_cast<T&>(*this);
        }

        /**
         * Sets the category bits used for collision filtering.
         * 
         * @param bits  the category bits
         * @return reference to this builder for method chaining
         */
        T& CategoryBits(uint16_t bits) {
            categoryBits = bits;
            return static_cast<T&>(*this);
        }

        /**
         * Sets the mask bits used for collision filtering.
         * 
         * This bitfield mask the categories that the collider does accept 
         * for collision.
         * 
         * @param bits  the mask bits
         * @return reference to this builder for method chaining
         */
        T& MaskBits(uint16_t bits) {
            maskBits = bits;
            return static_cast<T&>(*this);
        }

        /**
         * Sets the offset of the collider to build.
         * 
         * @param o the offset relativ to the origin of the entity
         * @return reference to this builder for method chaining
         */
        T& Offset(const Vector2f& o) {
            offset = o;
            return static_cast<T&>(*this);
        }        


    protected:

        /**
         * Configures the specified collider.
         * 
         * @param collider  the collider to configure
         */
        void Configure(CBodyCollider& collider) {
            collider.SetRestitution(restitution);
            collider.SetDensity(density);
            collider.SetFriction(friction);
            collider.SetCategoryBits(categoryBits);
            collider.SetMaskBits(maskBits);
            collider.SetOffset(offset);
        }

    private:
        /** The coefficient of restitution. */
        float restitution;

        /** The friction coefficient, usually in the range [0,1]. */
        float friction;

	    /** The density, usually in kg/m^2. */
    	float density;

        /** The collision category bits */
        uint16_t categoryBits;

        /** The collision mask bits. */
        uint16_t maskBits;

        /** The offset of this collider relative to the origin of its entity. */
        Vector2f offset;
    };


    /////////////////////////////////////////////////
    /////// CCircleCollider
    /////////////////////////////////////////////////

    /**
     * Circular collider.
     * 
     * @ingroup suite2d_group
     */
    class CCircleCollider : public CBodyCollider {
    public: 

        /**
         * Constructor.
         */
        CCircleCollider() 
        {
            // Intentionally left empty.        
        }

        /**
         * Returns the radius of this circle collider.
         * 
         * @return the radius
         */
        float GetRadius() const {
            return radius;
        }

        /**
         * Sets the radius of this circle collider.
         * 
         * @param r the radius of this collider
         * @throws std::domain_error in case the radius is less or equal zero
         */
        virtual void SetRadius(float r) {
            if (r <= 0) {
                throw std::domain_error(
                    "Radius of circle collider must be greater zero");
            }
            radius = r;
        }

        // Inherited via CBodyCollider
        virtual void OnAddedToEntity(Entity & entity)
        {
            entity.AddInterface(*this, typeid(CBodyCollider));
        }

    private:
        /** The radius of this circle collider. */
        float radius;
    };

    /**
     * Abstract factory for for CCircleCollider components.
     * 
     * @ingroup suite2d_group
     */
    class CCircleColliderFactory {
    public:

        /** Virtual destructor. */
        virtual ~CCircleColliderFactory() {}

        /**
         * Creates a new CCircleCollider instance.
         */
        virtual std::shared_ptr<CCircleCollider> CreateCircleCollider() = 0;
    };


    /////////////////////////////////////////////////
    /////// CCircleColliderBuilder
    /////////////////////////////////////////////////

    /**
     * Builds CCircleCollider instances.
     * 
     * @ingroup suite2d_group
     */
    class CCircleColliderBuilder 
        : public CBodyColliderBuilder<CCircleColliderBuilder>
    {
    public:

        /** 
         * Constructor.
         * 
         * If the specified collider factory is null a service which implements
         * the collider factory interface will be used.
         * 
         * @param colliderFactory   the factory used to create new instances
         */
        CCircleColliderBuilder(std::shared_ptr<CCircleColliderFactory> colliderFactory = nullptr);

        /**
         * Sets the radius of the circle collider to build.
         * 
         * @param r the radius of the collider
         * @throws std::domain_error in case the radius is less or equal zero
         */
        CCircleColliderBuilder& Radius(float r) {
            if (r <= 0) {
                throw std::domain_error(
                    "Radius of circle collider must be greater zero");
            }
            radius = r;
            return *this;
        }

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        CCircleColliderBuilder& Reset() {
            CBodyColliderBuilder::Reset();
            radius = 1.0f;
            return *this;
        }

        /**
         * Builds a new circle collider according to the current configuration.
         * 
         * @return the newly created circle collider
         */
        std::shared_ptr<CCircleCollider> Build();

    private:
        /** The circle collider factory used to create new instance. */
        std::shared_ptr<CCircleColliderFactory> colliderFactory;

        /** The radius of the circle collider to build. */
        float radius;
    };


    /////////////////////////////////////////////////
    /////// CPolygonCollider
    /////////////////////////////////////////////////

    /**
     * Polygonal collider.
     * 
     * @ingroup suite2d_group
     */
    class CPolygonCollider : public CBodyCollider {
    public: 

        /**
         * Constructor.
         */
        CPolygonCollider() 
        {
            // Intentionally left empty.        
        }

        /**
         * Sets the polygon of this collider.
         * 
         * @param poly  the polygon
         */
        void SetPolygon(std::shared_ptr<const Polygon2f> poly)
        {
            polygon = poly;
        }

        // Inherited via CBodyCollider
        virtual void OnAddedToEntity(Entity & entity)
        {
            entity.AddInterface(*this, typeid(CBodyCollider));
        }

    protected:
        /** The polygon defining the shape of this collider. */
        std::shared_ptr<const Polygon2f> polygon;
    };    

    /**
     * Abstract factory for for CPolygonCollider components.
     * 
     * @ingroup suite2d_group
     */
    class CPolygonColliderFactory {
    public:

        /** Virtual destructor. */
        virtual ~CPolygonColliderFactory() {}

        /**
         * Creates a new CCircleCollider instance.
         */
        virtual std::shared_ptr<CPolygonCollider> CreatePolygonCollider() = 0;
    };


    /////////////////////////////////////////////////
    /////// CPolygonColliderBuilder
    /////////////////////////////////////////////////

    /**
     * Builds CPolygonCollider instances.
     * 
     * @ingroup suite2d_group
     */
    class CPolygonColliderBuilder 
        : public CBodyColliderBuilder<CPolygonColliderBuilder>
    {
    public:

        /** 
         * Constructor.
         * 
         * If the specified collider factory is null a service which implements
         * the collider factory interface will be used.
         * 
         * @param colliderFactory   the factory used to create new instances
         */
        CPolygonColliderBuilder(std::shared_ptr<CPolygonColliderFactory> colliderFactory = nullptr);

        /**
         * Sets the polygon of the polygon collider to build.
         * 
         * @param poly  the polygon of the collider
         * @return reference to this builder for method chaining
         */
        CPolygonColliderBuilder& Polygon(std::shared_ptr<const Polygon2f> poly) {
            polygon = poly;
            return *this;
        }

        /**
         * Sets the polygon of the polygon collider to build.
         * 
         * @param vertices  the vertices that make up the polygon
         * @return reference to this builder for method chaining
         */
        CPolygonColliderBuilder& Polygon(const std::vector<Vector2f>& vertices) {
            polygon = std::make_shared<Polygon2f>(vertices);
            return *this;
        }

        /**
         * Set the polygon to an axis aligned rectangle.
         * 
         * @param width the width of the rectangle
         * @param height the height of the rectangle
         * @return reference to this builder for method chaining
         */
        CPolygonColliderBuilder& MakeRectangle(float width, float height);

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        CPolygonColliderBuilder& Reset() {
            CBodyColliderBuilder::Reset();
            polygon = nullptr;
            return *this;
        }

        /**
         * Builds a new polygon collider according to the current configuration.
         * 
         * @return the newly created polygon collider
         */
        std::shared_ptr<CPolygonCollider> Build();

    private:
        /** Used to build polygonal shapes. */
        static std::vector<Vector2f> tempVertices;

        /** The polygon collider factory used to create new instance. */
        std::shared_ptr<CPolygonColliderFactory> colliderFactory;

        /** The polygon defining the shape of the collider to build. */
        std::shared_ptr<const Polygon2f> polygon;
    };

} // end of namespace