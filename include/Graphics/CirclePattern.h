/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Pattern.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    /**
     * A pattern representing a circular shape.
     */
    class CirclePattern : public Pattern {
    public:

        /**
         * Constructor.
         *
         * @param r the radius of this circular shape
         */
        CirclePattern(double r = 1);

        /**
         * Sets the radius of this circular shape.
         *
         * @param r the radius
         * @throws std::domain_error in case the radius is less or equal zero
         */
        void SetRadius(double r);

        /**
         * Returns the radius of this circular shape.
         *
         * @return the radius
         */
        double GetRadius() const {
            return radius;
        }

        /**
         * Sets the pattern of this rectangle.
         *
         * @param p the pattern
         */
        void SetPattern(std::shared_ptr<Pattern> p) {
            pattern = p;
        }

        /**
         * Returns the pattern of this rectangle.
         *
         * @return  the pattern
         */
        std::shared_ptr<Pattern> GetPattern() {
            return pattern;
        }

        /**
         * Returns whether a pattern has been specified by this rectangle.
         *
         * @return `true` if a pattern has been specified
         */
        bool hasPattern() const {
            return pattern != nullptr;
        }

    protected:
        // Inherited via pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        /** The radius of this circular shape. */
        double radius;

        /** The radius of this circular shape squared. */
        double radiusSquared;

        /** The pattern of this rectangle. */
        std::shared_ptr<Pattern> pattern;
    };

} // end of namespace