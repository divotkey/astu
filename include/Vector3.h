/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <cmath>

namespace astu {

    /**
	 * A three dimensional vector. 
	 */
    template <typename T>
	class Vector3 {
    public:
        /** The x-coordinate of this vector. */
        T x;

        /** The y-coordinate of this vector. */
        T y;

        /** The z-coordinate of this vector. */
        T z;

        /**
         * Constructor.
         * 
         * @param vx	the x-coordinate
         * @param vy	thy y-coordinate
         * @param vz	thy z-coordinate
         */
        Vector3(T vx = 0, T vy = 0, T vz = 0)
            : x(vx), y(vy), z(vz)
        {
            // intentionally left empty
        }

        /**
         * Returns the length of this vector squared.
         *
         * @return the squared length of this vector
         */
        T LengthSquared() const {
            return x * x + y * y + z * z;
        }

        /**
         * Returns the length of this vector.
         * 
         * This method uses a square root function and is computationally
         * expensive. If possible use `lengthSquared` instead.
         *
         * @return the length of this vector
         */
        T Length() const {
            return std::sqrt(x * x + y * y + z * z);
        }        

    };

} // end of namespace