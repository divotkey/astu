/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Vector2.h"
#include "Matrix3.h"

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
    class Ray2 {
    public:

        /**
         * Creates a new ray object defined by two points.
         *
         * @param p0    the origin of the ray in world space
         * @param p1    a point on the ray in world space
         * @return the newly created ray with normalized direction vector
         */
        static Ray2 CreateFromTwoPoints(const Vector2<T> &p0, const Vector2<T> &p1) {
            return Ray2<T>(p0, astu::Normalize(p1 - p0));
        }

        /**
         * Constructor.
         * 
         * @param p the origin
         * @param d the direction vector
         */
        Ray2(const Vector2<T>& p, const Vector2<T>& d)
            : p0(p), dir(d)
        {
            // Intentionally left empty.
        }

        /**
         * Constructor.
         * 
         * @param px    the x-coordinate of the origin
         * @param py    the y-coordinate of the origin
         * @param dx    the x-component of the direction vector
         * @param dy    the y-component of the direction vector
         */
        Ray2(T px, T py, T dx, T dy)
            : p0(px, py), dir(dx, dy)
        {
            // Intentionally left empty.
        }

        /**
         * Constructs a ray from a given ray and transformation matrix.
         *
         * @param r the ray to transform
         * @param m the transformation matrix
         */
        Ray2(const Ray2<T>& r, const Matrix3<T> &m)
                : p0(m.TransformPoint(r.p0)), dir(m.TransformVector(r.dir))
        {
            // Intentionally left empty.
        }

        /**
         * Returns the origin of this ray.
         * 
         * @return this ray's origin
         */
        const Vector2<T>& GetOrigin() const {
            return p0;
        }

        /**
         * Sets the origin of this ray.
         *
         * @param p the new origin
         */
        void SetOrigin(const Vector2<T> p) {
            p0 = p;
        }

        /**
         * Returns the direction vector of this ray.
         * 
         * @return dir
         */
        const Vector2<T>& GetDirection() const {
            return dir;
        }

        /**
         * Returns a point on this ray.
         *
         * @param s the scaling factor used to calculate the point
         * @return the requested point on this ray
         */
        Vector2<T> GetPoint(T s) const {
            return p0 + dir * s;
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

        /**
         * Normalizes the direction vector of this ray.
         */
        void Normalize() {
            dir.Normalize();
        }

        /**
         * Transforms this ray by the specified transformation matrix.
         *
         * @param m the transformation matrix
         */
        void Transform(const Matrix3 <T> &m) {
            p0 = m.TransformPoint(p0);
            dir = m.TransformVector(dir);
        }

    private:
        /** The origin of this ray. */
        Vector2<T> p0;

        /** The direction of this ray. */
        Vector2<T> dir;
    };

    /**
     * Type alias for Ray2 template using double as data type.
     * 
     * @ingroup math_group
     */
    using Ray2d = astu::Ray2<double>;    

    /**
     * Type alias for Ray2 template using float as data type.
     * 
     * @ingroup math_group
     */
    using Ray2f = astu::Ray2<float>;    

} // end of namespace