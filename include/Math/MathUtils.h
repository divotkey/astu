/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Matrix4.h"

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
         * Compares two real numbers to be equal within a certain margin or error.
         *
         * @param a			the first real number
         * @param b			the second real number
         * @param epsilon	the maximum allowed difference
         * @return `true` if the two real number are considered to be equal
         */
        template <typename T>
        static T IsEqual(T a, T b, T epsilon) {
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

        template <typename T>
        static int CalcQuadricRoots(T a, T b, T c, T& x1, T& x2) {
            T determinant = b * b - 4 * a * c;
            if (determinant < 0) {
                return 0;
            }

            determinant = std::sqrt(determinant);
            T q = static_cast<T>(-0.5) * (b + copysign(determinant, b));

            x1 = q / a;
            x2 = c / q;

            // Uncomment t sort results.
            //if (x1 > x2) {
            //    q = x2; x2 = x1; x1 = q;
            //}
            if (x1 == x2) {
                return 1;
            }
            return 2;
        }

        /**
         * Returns the reflection vector given an incidence vector and a normal vector.
         *
         * The normal vector n should be normalized. If n is normalized, the output
         * vector will have the same length as the input incidence vector i.
         *
         * @param i     the incidence vector
         * @param nv    the normal vector
         * @return the reflection vector
         * @tparam T the numerical type of the vectors
         */
        template <typename T>
        static Vector3<T> Reflect(const Vector3<T> &i, const Vector3<T> &nv) {
            //return i - (static_cast<T>(2) * nv * nv.Dot(i));
            return i - nv * (static_cast<T>(2) * nv.Dot(i));
        }

        //TODO document LookAt method
        //TODO optimize and beautify LookAt method
        template <typename T>
        static Matrix4<T> LookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up) {
            Vector3<T> f = center - eye;
            f.Normalize();

            Vector3<T> s(f);
            s.Cross(up).Normalize();

            Vector3<T> u(s);
            u.Cross(f);

            Matrix4<T> result( s.x,  u.x, -f.x, 0,
                               s.y,  u.y, -f.y, 0,
                               s.z,  u.z, -f.z, 0,
                                 0,    0,    0, 1);

            Matrix4<T> tx;
            tx.SetToTranslate(-eye);
            result *= tx;
            return result;
        }

        //TODO document CreatePerspective method
        //TODO optimize and beautify CreatePerspective method
        template <typename T>
        static Matrix4<T> CreatePerspective(T near, T far, T fovy, T aspect)
        {
            assert(near > 0);
            assert(far > near);

            //T fd = 1.0 / tan((fovy * (Math.PI / 180)) / 2.0);
            T fd = 1.0 / tan(astu::ToRadians<T>(fovy) / 2.0);
            T a1 = (far + near) / (near - far);
            T a2 = (2 * far * near) / (near - far);

            return Matrix4<T>(fd / aspect,   0,   0,  0,
                                        0,  fd,   0,  0,
                                        0,   0,  a1, -1,
                                        0,   0,  a2,  0);
        }
    };

} // end of namespace