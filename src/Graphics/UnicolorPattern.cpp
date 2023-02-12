/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/UnicolorPattern.h"
#include "Math/Random.h"

// C++ Standard Library includes
#include <string>

namespace astu {

    std::shared_ptr<UnicolorPattern> UnicolorPattern::CreateRandomColored(double alpha)
    {
        auto &rnd = Random::GetInstance();
        return std::make_shared<UnicolorPattern>(Color4d(rnd.NextDouble(), rnd.NextDouble(), rnd.NextDouble(), alpha));
    }

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

    bool UnicolorPattern::IsInsideTransformed(const Vector2<double> &pt) const
    {
        return true;
    }


} // end of namespace