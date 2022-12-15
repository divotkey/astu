/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/CirclePattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    CirclePattern::CirclePattern(double r)
    {
        SetRadius(r);
    }

    void CirclePattern::SetRadius(double r)
    {
        if (r <= 0) {
            throw std::domain_error("Circle radius must be greater zero, got " + std::to_string(r));
        }
        radius = r;
        radiusSquared = r * r;
    }

    bool CirclePattern::GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const
    {
        if (pt.LengthSquared() > radiusSquared) {
            return false;
        }

        if (pattern) {
            return pattern->GetColor(pt, outColor);
        }
        return false;
    }

    BoundingBox CirclePattern::GetLocalBoundingBox() const
    {
        return BoundingBox(radius * 2, radius * 2);
    }

} // end of namespace