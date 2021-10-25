/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <cmath>

namespace astu {

    template <typename T>
    T ToRadians(T phi) {
        return phi * static_cast<T>(3.14159265358979323846 / 180.0);
    }

    template <typename T>
    T ToDegrees(T phi) {
        return phi * static_cast<T>(180.0 / 3.14159265358979323846);
    }

    /**
     * This class provides a collection of auxiliary mathematical functions and constants.
     * 
     * @ingroup math_group
     */
    class MathUtils final {
    public:
        /** Constant for PI with double precision. */
        const static double PId;

        /** Constant for PI times two with double precision. */
        const static double PI2d;

        /** Constant for PI with single precision. */
        const static float PIf;

        /** Constant for PI times two with single precision. */
        const static float PI2f;

        /**
         * Converts from degrees to radians.
         * 
         * @param phi   the angle in degrees
         */
        template <typename T>
        static T ToRadians(T phi) {
            return phi * static_cast<T>(3.14159265358979323846 / 180.0);
        }

        /**
         * Converts from radians to degrees.
         * 
         * @param phi   the angle in radians
         */
        template <typename T>
        static T ToDegrees(T phi) {
            return phi * static_cast<T>(180.0 / 3.14159265358979323846);
        }

        /**
         * Calculates the diagonal of a two dimensional square.
         * 
         * @param width the width of the square
         * @param height    the height of the square
         * @return the length of the diagonal of the specified square
         * @tparam  the numerical type
         */
        template <typename T>
        static T RectDiag2D(T width, T height) {
            return std::sqrt(width * width + height * height);
        }

        /**
         * Snaps the specified value to a grid with given width.
         * 
         * @param value     the value to snap
         * @param gridWidth the width of the grid
         */
        template <typename T>
        static T Snap(T value, T gridWidth) {
            return std::round(value / gridWidth) * gridWidth;
        }

        /**
         * Exponentiation by squaring.
         * 
         * This functions reises the parameter b to the power of e, comparable
         * to the pow function of the C++ standard library. However, this
         * funtions operates on integer values rather than floating point
         * values.
         * 
         * For more details of the algorithm see:
         * https://en.wikipedia.org/wiki/Exponentiation_by_squaring
         * 
         * @param b the basis
         * @param e the exponent (must be positive)
         * @TParam T
         */
        template <typename T>
        static T BinExp(T b, T e) {
            if (!e) return T(1);

            T h = ~(~0u >> 1); 
            while (!(e & h)) {
                h >>= 1;
            }

            T r = b;

            while (h >>= 1)
            {
                r *= r;
                if (e & h) {
                    r *= b; 
                }
            }

            return r;
        }

    };

} // end of namespace