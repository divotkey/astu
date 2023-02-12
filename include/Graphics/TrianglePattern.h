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
    class TrianglePattern : public Pattern {
    public:

        /**
         * Constructor.
         */
        TrianglePattern() = default;

        /**
         * Constructor.
         *
         * @param p0    the first point of the triangle
         * @param p1    the second point of the triangle
         * @param p2    the third point of the triangle
         */
        TrianglePattern(const Vector2d &p0, const Vector2d &p1, const Vector2d &p2);

        void SetPoint0(const Vector2d &p);
        void SetPoint1(const Vector2d &p);
        void SetPoint2(const Vector2d &p);

        const Vector2d &GetPoint0() const;

        const Vector2d &GetPoint1() const;

        const Vector2d &GetPoint2() const;

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
        bool HasPattern() const {
            return pattern != nullptr;
        }

    protected:

        // Inherited via pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

        bool IsInsideTransformed(const Vector2<double> &pt) const override;

    private:
        Vector2d p0;
        Vector2d p1;
        Vector2d p2;

        /** The pattern of this rectangle. */
        std::shared_ptr<Pattern> pattern;
    };

} // end of namespace