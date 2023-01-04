/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/UnionPattern.h"

namespace astu {

    class BlurPattern : public UnionPattern {
    public:

        /**
         * Constructor.
         *
         * @param radius    the blur radius
         * @oaran nSamples  number of samples used to estimate the blurred result.
         */
        BlurPattern(double r = 1, unsigned int nSamples = 64);

        /**
         * Sets the number of samples used to estimate the blurred result.
         *
         * @param n number of samples
         */
        void SetNumSamples(unsigned int n);

        /**
         * Returns the number of samples used to estimate the blurred result.
         *
         * @return number of samples
         */
        unsigned int GetNumSamples() const;

        /**
         * Returns the radius use to blur added sub-patterns.
         *
         * @return the blur radius
         */
        double GetRadius() const {
            return radius;
        }

        /**
         * Sets the radius use to blur added sub-patterns.
         *
         * @param r the radius
         * @throws std::domain_error in case the radius is less or equal zero
         */
        void SetRadius(double r);

    protected:

        // Inherited via CompoundPattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        /** The blur radius. */
        double radius;

        /** The number of samples used to estimate the blurred result. */
        unsigned int nSamples;
    };

}