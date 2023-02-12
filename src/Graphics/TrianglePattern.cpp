/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/TrianglePattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <algorithm>
#include <string>

namespace astu {

    TrianglePattern::TrianglePattern(const Vector2d &p0, const Vector2d &p1, const Vector2d &p2)
        : p0(p0), p1(p1), p2(p2)
    {
        // Intentionally left empty.
    }

    bool TrianglePattern::GetColorTransformed(const Vector2<double> &pt, Color4d & outColor) const
    {
        if (pattern && IsInsideTransformed(pt))
            return pattern->GetColor(pt, outColor);

        return false;

        //double s = (p0.x - p2.x) * (pt.y - p2.y) - (p0.y - p2.y) * (pt.x - p2.x);
        //double t = (p1.x - p0.x) * (pt.y - p0.y) - (p1.y - p0.y) * (pt.x - p0.x);
        //
        //if ((s < 0) != (t < 0) && s != 0 && t != 0)
        //    return false;
        //
        //double d = (p2.x - p1.x) * (pt.y - p1.y) - (p2.y - p1.y) * (pt.x - p1.x);
        //if (d == 0 || (d < 0) == (s + t <= 0)) {
        //    return pattern->GetColor(pt, outColor);
        //}
        //
        //return false;
    }

    bool TrianglePattern::IsInsideTransformed(const Vector2<double> &pt) const
    {
        double s = (p0.x - p2.x) * (pt.y - p2.y) - (p0.y - p2.y) * (pt.x - p2.x);
        double t = (p1.x - p0.x) * (pt.y - p0.y) - (p1.y - p0.y) * (pt.x - p0.x);

        if ((s < 0) != (t < 0) && s != 0 && t != 0)
            return false;

        double d = (p2.x - p1.x) * (pt.y - p1.y) - (p2.y - p1.y) * (pt.x - p1.x);
        return d == 0 || (d < 0) == (s + t <= 0);
    }


    BoundingBox TrianglePattern::GetLocalBoundingBox() const 
    {
        BoundingBox bbox;

        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();

        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::lowest();

        minX = std::min(minX, p0.x);
        maxX = std::max(maxX, p0.x);
        minY = std::min(minY, p0.y);
        maxY = std::max(maxY, p0.y);

        minX = std::min(minX, p1.x);
        maxX = std::max(maxX, p1.x);
        minY = std::min(minY, p1.y);
        maxY = std::max(maxY, p1.y);

        minX = std::min(minX, p2.x);
        maxX = std::max(maxX, p2.x);
        minY = std::min(minY, p2.y);
        maxY = std::max(maxY, p2.y);

        bbox.SetWidth(maxX - minX);
        bbox.SetHeight(maxY - minY);
        bbox.SetCenter({minX + bbox.GetWidth() * 0.5, minY + bbox.GetHeight() * 0.5});

        return bbox;
    }

    void TrianglePattern::SetPoint0(const Vector2d &p)
    {
        p0 = p;
    }

    void TrianglePattern::SetPoint1(const Vector2d &p)
    {
        p1 = p;
    }

    void TrianglePattern::SetPoint2(const Vector2d &p)
    {
        p2 = p;
    }

    const Vector2d &TrianglePattern::GetPoint0() const
    {
        return p0;
    }

    const Vector2d &TrianglePattern::GetPoint1() const
    {
        return p1;
    }

    const Vector2d &TrianglePattern::GetPoint2() const
    {
        return p2;
    }

} // end of namespace