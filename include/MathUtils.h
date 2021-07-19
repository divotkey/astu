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

        template <typename T>
        static T ToRadians(T phi) {
            return phi * static_cast<T>(3.14159265358979323846 / 180.0);
        }

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

    };

} // end of namespace