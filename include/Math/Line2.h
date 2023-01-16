/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Vector2.h"

// C++ Standard Library includes
#include <optional>

namespace astu {

    /**
     * A two-dimensional line. 
     * 
     * This class is a mathematical utility class for two-dimensional lines.
     * It is a straight line, infinitely long, infinitely thin, and unlimited
     * in both directions. The shortest connection between two points, on the
     * other hand, is called a line segment. 
     * 
     * @tparam T    the numerical data type used for this line
     * 
     * @ingroup math_group
     */
    template <typename T>
    class Line2 {
    public:

        /**
         * Calculates the intersection between two lines.
         * 
         * This algorithm calculates the scaling factors of the direction
         * vectors for each line. The scaling factors mark the intersection
         * point on the two lines.
         * 
         * The intersection point can be determined by either
         * pA + vA * s1 or pB + vB * s2.
         * 
         * @param pA    the origin of the first line
         * @param vA    the direction of the first line
         * @param pB    the origin of the second line
         * @param vB    the direction of the second line
         * @param outS1 receives the scaling factor for the first line
         * @param outS2 receives the scaling factor for the second line
         * @return  `true` if the two lines intersect or false if the two lines 
         *          are parallel
         */
        static bool Intersect(
            const Vector2<T>& pA, 
            const Vector2<T>& vA,
            const Vector2<T>& pB, 
            const Vector2<T>& vB,
            T &outS1,
            T &outS2)
        {
            T d =  vA.x * vB.y - vA.y * vB.x;

            if (d == 0) {
                return false;
            }

            outS1 = (pA.y * vB.x - pB.y * vB.x - pA.x * vB.y + pB.x * vB.y) / d;
            outS2 = (pA.y * vA.x - pB.y * vA.x - pA.x * vA.y + pB.x * vA.y) / d;
            return true;
        }

        /**
         * Constructor.
         * 
         * @param p a point on the line
         * @param d the direction of the line
         */
        Line2(const Vector2<T>& p, const Vector2<T>& d)
            : p0(p), dir(d)
        {
            // Intentionally left empty.
        }

        /**
         * Returns the origin of this line.
         * 
         * @return the origin of this line
         */
        const Vector2<T>& GetOrigin() const {
            return p0;
        }

        /**
         * Returns the direction vector of this line.
         * 
         * @return dir
         */
        const Vector2<T>& GetDirection() const {
            return dir;
        }

        /**
         * Intersects this line with the specified line.
         * 
         * This method calculates the scaling factor of the direction vector of
         * this line such that it marks the intersection point. The intersection 
         * point can be determined by either p0 + v * s, where p0 represents the
         * line's origin, and v is the direction vector.         
         * 
         * @param other the other line
         * @param outS1 receives the scaling factor for this line
         * @return  'true` if the two lines intersect or false if the two lines 
         *          are parallel
         */
        bool Intersect(const Line2<T>& other, T& outS1) const {
            T s2;
            return Intersect(
                GetOrigin(), 
                GetDirection(), 
                other.GetOrigin(), 
                other.GetDirection(), 
                outS1, 
                s2);
        }

        /**
         * Intersects this line with the specified line.
         * 
         * @param other the other line
         * @param outPoint receives the intersection point
         * @return  'true` if the two lines intersect or false if the two lines 
         *          are parallel
         */
        bool Intersect(const Line2<T>& other, Vector2<T>& outPoint) const {
            T s1, s2;
            bool valid = Intersect(
                GetOrigin(), 
                GetDirection(), 
                other.GetOrigin(), 
                other.GetDirection(), 
                s1, 
                s2);

            if (valid) {
                outPoint = GetOrigin() + GetDirection() * s1;
                return true;
            }

            return false;
        }

        /**
         * Calculates the distance between this line and a given point.
         *
         * @param p the point in question
         * @return the distance
         */
        T CalcDistance(const Vector2<T> &p) const {
            Line2<T> tmp(p, Vector2<T>(-dir.y, dir.x).Normalize());

            double s;
            if (tmp.Intersect(*this, s)) {
                return std::abs(s);
            }
            return std::numeric_limits<T>::max();
        }

    private:
        /** The starting point of this line. */
        Vector2<T> p0;

        /** The direction of this line. */
        Vector2<T> dir;
    };

    /**
     * Type alias for Line2 template using double as data type.
     * 
     * @ingroup math_group
     */
    using Line2d = astu::Line2<double>;    

    /**
     * Type alias for Segment1 template using float as data type.
     * 
     * @ingroup math_group
     */
    using Line2f = astu::Line2<float>;    


} // end of namespace