/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Vector3.h"

namespace astu {

    /**
     * Represents spherical coordinates in three-dimensional space.
     *
     * @tparam T the numerical type used to store the coordinates
     * @ingroup math_group
     */
    template <typename T>
    class SphericalCoordinates {
    public:
        /** The polar angle. */
        T theta;

        /** The azimuthal angle. */
        T phi;

        /** The radius. */
        T radius;

        /**
         * Creates a new SphericalCoordinates object from given Cartesian coordinates.
         *
         * @param v the Cartesian coordinates
         * @return the newly created spherical coordinates
         */
        static SphericalCoordinates FromCartesian(const Vector3<T> &v) {
            T r = v.Length();
            T phi = std::atan2(v.z, v.x);
            if (phi < 0)
                phi += MathUtils::PI2d;

            return SphericalCoordinates(phi,
                                        acos(v.y / r),
                                        r);
        }

        /**
         * Constructor.
         */
        SphericalCoordinates() : phi(0), theta(0), radius(1) {
            // Intentionally left empty
        }

        /**
         * Constructor.
         *
         * @param phi       the polar angle
         * @param theta     the azimuthal angle
         * @param radius    the radius
         */
        SphericalCoordinates(T phi, T theta, T radius) : phi(phi), theta(theta), radius(radius) {
            // Intentionally left empty
        }
    };

} // end of namespace

