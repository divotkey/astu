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
     * A pattern representing a rectangular shape.
     */
    class RectanglePattern : public Pattern {
    public:

        /**
         * Constructs an axis aligned rectangular shape
         *
         * @param w the width of this rectangle
         * @param h the height of this rectangle
         */
        RectanglePattern(double w = 1, double h = 1);

        /**
         * Sets the width of this rectangle.
         *
         * @param w the width
         * @throws std::domain_error in case the width is less or equal zero
         */
        void SetWidth(double w);

        /**
         * Sets the height of this rectangle.
         *
         * @param w the height
         * @throws std::domain_error in case the height is less or equal zero
         */
        void SetHeight(double h);

        /**
         * Sets the dimensions of this rectangle.
         *
         * @param w the width of this rectangle
         * @param h the height of this rectangle
         * @throws std::domain_error in case the width or height height is less or equal zero
         */
        void SetSize(double w, double h) {
            SetWidth(w);
            SetHeight(h);
        }

        /**
         * Returns the width of this rectangle.
         *
         * @return the width
         */
        double GetWidth() const {
            return width;
        }

        /**
         * Returns the height of this rectangle.
         *
         * @return the height
         */
        double GetHeight() const {
            return height;
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
        /** The width of this rectangle. */
        double width;

        /** The height of this rectangle. */
        double height;

        /** The horizontal radius (width / 2) of this rectangle. */
        double hRadius;

        /** The vertical radius (height / 2) of this rectangle. */
        double vRadius;

        /** The pattern of this rectangle. */
        std::shared_ptr<Pattern> pattern;
    };

} // end of namespace