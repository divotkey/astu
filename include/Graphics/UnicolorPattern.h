/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Pattern.h"
#include "Graphics/WebColors.h"


// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    /**
     * An infinite pattern representing one single color.
     */
    class UnicolorPattern : public Pattern {
    public:

        /**
         * Constructor.
         *
         * @param c the one and only color of this pattern
         */
        UnicolorPattern(const Color4d &c = WebColors::Cyan) : color(c) {}

        /**
         * Returns the one and only color of this pattern.
         *
         * @return the pattern's color
         */
        const Color4d &GetColor() const {
            return color;
        }

        /**
         * Sets the one and only color of this pattern.
         *
         * @param c the one and only color of this pattern
         */
        void SetColor(const Color4d &c) {
            color = c;
        }

        // Inherited via Pattern
        virtual bool GetColor(const Vector2<double> &p, Color4d &outColor) const override;

    protected:

        // Inherited via Pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        /** The one and only color of this pattern. */
        Color4d color;
    };

} // end of namespace