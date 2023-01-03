/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <cmath>
#include <utility>

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

        /** Constant for PI divided by  four with double precision. */
        const static double PIOver2d;

        /** Constant for PI divided by  four with double precision. */
        const static double PIOver4d;

        /** Inverse PI (1 / PI) with double precision. */
        const static double InvPId;

        /** Inverse of PI times two (1 / 2PI) with double precision. */
        const static double InvPI2d;

        /** Constant for PI with single precision. */
        const static float PIf;

        /** Constant for PI times two with single precision. */
        const static float PI2f;

        /** Constant for PI divided by two with double precision. */
        const static float PIOver2f;

        /** Constant for PI divided by two with double precision. */
        const static float PIOver4f;

        /** Inverse PI (1 / PI) with single precision. */
        const static float InvPIf;

        /** Inverse of PI times two (1 / 2PI) with double precision. */
        const static float InvPI2f;

        /** Default error margin used to compare floating-point values. */
        const static double Epsilon;

        // Deleted constructor -> no instances of this class allowed.
        MathUtils() = delete;
        MathUtils(const MathUtils&) = delete;
        MathUtils(MathUtils&&) = delete;

        // Delete assign operator -> non copyable
        MathUtils& operator=( const MathUtils& ) = delete;

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
         * Compares two floating-point numbers to be equal within a certain margin or error.
         *
         * @param a			the first floating-point number
         * @param b			the second floating-point number
         * @param epsilon	the maximum allowed difference
         * @return `true` if the two real number are considered to be equal
         */
        template <typename T>
        static T IsEqual(T a, T b, T epsilon = static_cast<T>(Epsilon)) {
            return std::abs(a - b) < epsilon;
        }

        /**
         * Clamps the specified value to the given range [min, max].
         *
         * @param value	the value to clamp
         * @param min	the lower boundary
         * @param max	the upper boundary
         */
        template <typename T>
        static T Clamp(T value, T min, T max) {
            if (value < min)
                return min;
            if (value > max)
                return max;
            return value;
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

        /**
         * Linear interpolation between two values.
         *
         * @param a the first value
         * @param b the second value
         * @param t interpolation factor [0, 1]
         * @return the interpolated value
         * @tparam T the numerical type of the values
         */
        template <typename T>
        static T Lerp(T a, T b, T t) {
            return a + t * (b - a);
        }

        /**
         * Linear interpolation between two values, that guarantees v = v1 when t = 1.
         *
         * @param a the first value
         * @param b the second value
         * @param t interpolation factor [0, 1]
         * @return the interpolated value
         * @tparam T the numerical type of the values
         */
        template <typename T>
        static T LerpPrecise(T a, T b, T t) {
            return (static_cast<T>(1) - t) * a + t * b;
        }

        template <typename T>
        static bool CalcQuadricRoots(T a, T b, T c, T& x1, T& x2) {
            T determinant = b * b - 4 * a * c;
            if (determinant < 0) {
                return false;
            }

            determinant = std::sqrt(determinant);
            T q = static_cast<T>(-0.5) * (b + copysign(determinant, b));

            x1 = q / a;
            x2 = c / q;

            // Make sure x1 is < x2
            if (x2 < x1) {
                std::swap(x1, x2);
            }
            return true;
        }

    };

} // end of namespace