/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Vector3.h"

// C++ Standard Library includes
#include <iostream>

namespace astu {

    /**
     * A two-dimensional ray. 
     * 
     * This class is a mathematical utility class representing a ray.
     * A ray is also called a half-line. A ray can be defined as a part of a
     * line, that has a fixed starting point but no end point.
     * It can extend infinitely in one direction. 
     * 
     * @tparam T    the numerical data type used for this ray
     * 
     * @ingroup math_group
     */
    template <typename T>
    class Ray3 {
    public:

        /**
         * Constructor.
         */
        Ray3()
        {
            // Intentionally left empty.
        }

        /**
         * Constructor.
         * 
         * @param p the origin of this ray
         * @param d the direction vector
         */
        Ray3(const Vector3<T>& p, const Vector3<T>& d)
            : p0(p), dir(d)
        {
            // Intentionally left empty.
        }

        /**
         * Constructor.
         * 
         * @param px    the x-coordinate of origin
         * @param py    the y-coordinate of origin
         * @param pz    the z-coordinate of origin
         * @param dx    the x-component of the direction vector
         * @param dy    the y-component of the direction vector
         * @param dz    the z-component of the direction vector
         */
        Ray3(T px, T py, T pz, T dx, T dy, T dz)
            : p0(px, py, pz), dir(dx, dy, dz)
        {
            // Intentionally left empty.
        }

        /**
         * Returns the origin of this ray.
         * 
         * @return this ray's origin
         */
        const Vector3<T>& GetOrigin() const {
            return p0;
        }

        /**
         * Sets the origin of this ray.
         *
         * @param p the new origin
         */
        void SetOrigin(const Vector3<T> p) {
            p0 = p;
        }

        /**
         * Returns the direction vector of this ray.
         * 
         * @return dir
         */
        const Vector3<T>& GetDirection() const {
            return dir;
        }

        /**
         * Sets the direction of this ray.
         *
         * @param d the direction vector
         */
        void SetDirection(const Vector3<T> d) {
            dir = d;
        }

        /**
         * Returns the length of this ray.
         *
         * @return this ray's length
         */
        T Length() const {
            return dir.Length();
        }

        /**
         * Returns the length of this ray.
         *
         * @return this ray's length
         */
        T LengthSquared() const {
            return dir.LengthSquared();
        }

    public:
        /** The origin of this ray. */
        Vector3<T> p0;

        /** The direction of this ray. */
        Vector3<T> dir;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Ray3<T> &ray) {
        os << ray.p0 << " -> " << ray.dir;
        return os;
    }

    /**
     * Type alias for Ray2 template using double as data type.
     * 
     * @ingroup math_group
     */
    using Ray3d = astu::Ray3<double>;

    /**
     * Type alias for Ray2 template using float as data type.
     * 
     * @ingroup math_group
     */
    using Ray3f = astu::Ray3<float>;

} // end of namespace