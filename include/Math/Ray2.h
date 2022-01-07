/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Vector2.h"

// C++ Standard Library includes
#include <optional>

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
         * Constructor.
         * 
         * @param p the starting point
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
         * @param px    the x-coordinate of the starting point
         * @param py    the y-coordinate of the starting point
         * @param dx    the x-component of the direction vector
         * @param dy    the y-component of the direction vector
         */
        Ray2(T px, T py, T dx, T dy)
            : p0(px, py), dir(dx, dy)
        {
            // Intentionally left empty.
        }

        /**
         * Returns the start point of this ray.
         * 
         * @return the start point
         */
        const Vector2<T>& GetStartPoint() const {
            return p0;
        }

        /**
         * Returns the direction vector of this ray.
         * 
         * @return dir
         */
        const Vector2<T>& GetDirection() const {
            return dir;
        }

    private:
        /** The starting point of this ray. */
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