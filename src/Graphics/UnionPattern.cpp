/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/UnionPattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    bool UnionPattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        bool hasColor = false;
        for (auto pattern : children) {
            Color4d localColor;
            if (pattern->GetColor(pt, localColor)) {
                if (hasColor) {
                    c.Blend(localColor);
                } else {
                    hasColor = true;
                    c = localColor;
                }
            }
        }

        return hasColor;
    }

    BoundingBox UnionPattern::GetLocalBoundingBox() const
    {
        BoundingBox result;

        for (const auto pattern : children) {
            result.Merge(pattern->GetBoundingBox());
        }

        return result;
    }

    bool UnionPattern::IsInsideTransformed(const Vector2<double> &pt) const
    {
        for (const auto pattern : children) {
            if (pattern->IsInside(pt))
                return true;
        }

        return false;
    }

} // end of namespace