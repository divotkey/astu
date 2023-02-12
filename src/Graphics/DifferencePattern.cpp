/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/DifferencePattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    bool DifferencePattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        if (children.empty())
            return false;

        auto it = children.begin();
        std::shared_ptr<Pattern> mainPattern = *it++;

        while (it != children.end()) {
            if ((*it++)->IsInside(pt))
                return false;
        }


        return mainPattern->GetColor(pt, c);
    }

    BoundingBox DifferencePattern::GetLocalBoundingBox() const
    {
        BoundingBox result;

        if (children.empty()) {
            return BoundingBox();
        }

        return children.front()->GetBoundingBox();
    }

    bool DifferencePattern::IsInsideTransformed(const Vector2<double> &pt) const
    {
        if (children.empty())
            return false;

        return children.front()->IsInside(pt);
    }

} // end of namespace