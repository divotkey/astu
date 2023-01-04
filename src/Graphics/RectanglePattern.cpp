/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/RectanglePattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    RectanglePattern::RectanglePattern(double w, double h)
    {
        SetWidth(w);
        SetHeight(h);
    }

    void RectanglePattern::SetWidth(double w)
    {
        if (w <= 0) {
            throw std::domain_error("Width of rectangle pattern must be greater zero, got " 
                + std::to_string(w));
        }
        width = w;
        hRadius = width / 2;
    }

    void RectanglePattern::SetHeight(double h)
    {
        if (h <= 0) {
            throw std::domain_error("Height of rectangle pattern must be greater zero, got " 
                + std::to_string(h));
        }
        height = h;
        vRadius = height / 2;
    }

    bool RectanglePattern::GetColorTransformed(const Vector2<double> &pt, Color4d & outColor) const
    {
        if (pt.x > hRadius || pt.x < -hRadius) {
            return false;
        }

        if (pt.y > vRadius || pt.y < -vRadius) {
            return false;
        }

        if (pattern) {
            return pattern->GetColor(pt, outColor);
        }
        
        return false;
    }

    BoundingBox RectanglePattern::GetLocalBoundingBox() const 
    {
        return BoundingBox(width, height);
    }

} // end of namespace