/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/UnicolorPattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    bool UnicolorPattern::GetColor(const Vector2<double> &p, Color4d &outColor) const
    {
        outColor = color;
        return true;
    }

    bool UnicolorPattern::GetColorTransformed(const Vector2<double> &pt, Color4d & outColor) const {
        // This method should actually never get called, because the only source which uses this
        // method is `GetColor(const Vector2<double> &p, Color4d &outColor)` and this method is
        // overwritten by this class.
        outColor = color;
        return true;
    }

    BoundingBox UnicolorPattern::GetLocalBoundingBox() const
    {
        return BoundingBox::CreateInfinite();
    }

}