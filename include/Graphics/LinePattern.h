/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Pattern.h"
#include "Graphics/RectanglePattern.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    /**
     * A pattern representing a circular shape.
     */
    class LinePattern : public Pattern {
    public:

        /**
         * Constructor.
         */
        LinePattern();

        /**
         * Sets the width of this line.
         *
         * @param w the width
         * @throws std::domain_error in case the width is less or equal zero
         */
        void SetWidth(double w);

        /**
         * Returns the width of this line.
         *
         * @return  this line's width
         */
        double GetWidth() const {
            return width;
        }

        /**
         * Sets the first point of this line.
         *
         * @param p the first point
         */
        void SetPoint1(const Vector2d &p) {
            SetPoint1(p.x, p.y);
        }

        /**
         * Sets the first point of this line.
         *
         * @param px    the x-coordinate of the first point
         * @param py    the y-coordinate of the first point
         */
        void SetPoint1(double px, double py);

        /**
         * Return the first point of this line.
         *
         * @return the second point
         */
        const Vector2d &GetPoint1() const {
            return point1;
        }

        /**
         * Sets the second point of this line.
         *
         * @param p the second point
         */
        void SetPoint2(const Vector2d &p) {
            SetPoint2(p.x, p.y);
        }

        /**
         * Sets the second point of this line.
         *
         * @param px    the x-coordinate of the second point
         * @param py    the y-coordinate of the second point
         */
        void SetPoint2(double px, double py);

        /**
         * Return the second point of this line.
         *
         * @return the second point
         */
        const Vector2d &GetPoint2() const {
            return point2;
        }

        /**
         * Sets the pattern of this rectangle.
         *
         * @param p the pattern
         */
        void SetPattern(std::shared_ptr<Pattern> p);

        /**
         * Returns the pattern of this line.
         *
         * @return  the pattern
         */
        std::shared_ptr<Pattern> GetPattern();

        /**
         * Returns whether a pattern has been specified by this line.
         *
         * @return `true` if a pattern has been specified
         */
        bool HasPattern() const;

    protected:
        // Inherited via pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        /** The first point of this line. */
        Vector2d point1;

        /** The first point of this line. */
        Vector2d point2;

        /** The width of this line. */
        double width;

        /** The pattern of this line. */
        std::shared_ptr<Pattern> pattern;

        /** The rectangle which resembles the line. */
        RectanglePattern rectangle;

        void UpdateLine();
    };

} // end of namespace