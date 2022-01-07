/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Vector2.h"
#include "Line2.h"
#include "Ray2.h"

// C++ Standard Library includes
#include <optional>

namespace astu {

    /**
     * A two-dimensional line segment. 
     * 
     * This class is a mathematical utility class to carry out some tests
     * with two-dimensional segments.
     * 
     * @tparam T    the numerical data type used for this segment
     * 
     * @ingroup math_group
     */
    template <typename T>
    class Segment2 {
    public:

        /**
         * Constructor.
         * 
         * @param p0    the start point of this line segment
         * @param p1    the end point of this line segment
         */
        Segment2(const Vector2<T>& p0, const Vector2<T>& p1)
            : p0(p0), p1(p1)
        {
            // Intentionally left empty.
        }

        /**
         * Gets the start point of this line segment.
         * 
         * @return the start point
         */
        const Vector2f& GetStartPoint() const {
            return p0;
        }

        /**
         * Gets the end point of this line segment.
         * 
         * @return the start point
         */
        const Vector2f& GetEndPoint() const {
            return p1;
        }

        /**
         * Returns the vector pointing from the start point to the end point.
         * 
         * @return the direction vector p1 - p0
         */
        Vector2f GetDirection() const {
            return p1 - p0;
        }

        /**
         * Tests whether this line segments intersects with the given segment.
         * 
         * @param other the other line segment
         * @return `true` if the two line segments intersect
         */
        bool IsIntersecting(const Segment2<T>& other) const {
            Vector2<T> notUsed;

            return Intersect(
                GetStartPoint(), 
                GetDirection(), 
                other.GetStartPoint(), 
                other.GetDirection(),
                notUsed);
        }

        /**
         * Tests whether this line segment intersects with the specified ray.
         * 
         * @param ray   the ray to test with
         * @return `true` if this line segment intersects with the ray
         */
        bool IsIntersecting(const Ray2<T>& ray) const {
            T s1, s2;
            bool valid = Line2<T>::Intersect(
                GetStartPoint(), 
                GetDirection(),
                ray.GetStartPoint(),
                ray.GetDirection(),
                s1,
                s2
                );

            return valid && s1 >= 0 && s1 <= 1 && s2 >= 0;
        }

        /**
         * Calculates the intersection point with the specified ray.
         * 
         * @param ray       the ray to intersect with
         * @param outPoint  receives the intersection point
         * @return `true` if an intersection exists
         */
        bool Intersect(const Ray2<T>& ray, Vector2<T>& outPoint) const {
            T s1, s2;
            bool valid = Line2<T>::Intersect(
                GetStartPoint(), 
                GetDirection(),
                ray.GetStartPoint(),
                ray.GetDirection(),
                s1,
                s2);

            if (valid && s1 >= 0 && s1 <= 1 && s2 >= 0) {
                outPoint = ray.GetStartPoint() + ray.GetDirection() * s2;
                return true;
            }

            return false;
        }

        /**
         * Tests whether this line segments intersects with the given segment.
         * 
         * @param other the other line segment
         * @return  an optional value, if set, represents the point of 
         *          intersection, if not set, the two line segments do not
         *          intersect
         */
        std::optional<astu::Vector2<T>> Intersect(const Segment2<T>& other) const {
            std::optional<Vector2<T>> result;
            Vector2<T> point;

            if (Intersect(other, point)) {
                result = point;
            }

            return result;
        }

        /**
         * Tests whether this line segments intersects with the given segment.
         * 
         * @param other     the other line segment
         * @param outPoint  receives the intersection point
         * @return `true` if the two line segnents intersect
         */
        bool Intersect(const Segment2<T>& other, Vector2<T>& outPoint) const {
            T s1, s2;
            bool valid = Line2<T>::Intersect(
                GetStartPoint(), 
                GetDirection(),
                other.GetStartPoint(),
                other.GetDirection(),
                s1,
                s2);

            if (valid && s1 >= 0 && s1 <= 1 && s2 >= 0 && s2 <= 1) {
                outPoint = GetStartPoint() + GetDirection() * s1;
                return true;
            }

            return false;
        }

    private:
        /** The start point of this segment. */
        Vector2<T> p0;

        /** The end point of this segment. */
        Vector2<T> p1;
    };

    /**
     * Type alias for Segment2 template using double as data type.
     * 
     * @ingroup math_group
     */
    using Segment2d = astu::Segment2<double>;    

    /**
     * Type alias for Segment1 template using float as data type.
     * 
     * @ingroup math_group
     */
    using Segment2f = astu::Segment2<float>;    

} // end of namespace