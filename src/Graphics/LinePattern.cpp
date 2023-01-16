/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/LinePattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    LinePattern::LinePattern()
        : point1(0, 0), point2(1, 0), width(0.1)
    {
        // Intentionally left empty.
    }

    bool LinePattern::GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const
    {
        return rectangle.GetColor(pt, outColor);
    }

    BoundingBox LinePattern::GetLocalBoundingBox() const
    {
        return rectangle.GetBoundingBox();
    }

    void LinePattern::SetPoint1(double px, double py)
    {
        point1.Set(px, py);
        UpdateLine();
    }

    void LinePattern::SetPoint2(double px, double py)
    {
        point2.Set(px, py);
        UpdateLine();
    }

    void LinePattern::SetWidth(double w)
    {
        if (w <= 0) {
            throw std::domain_error("Line width less or equal than zero not allowed, got " + std::to_string(w));
        }

        width = w;
        UpdateLine();
    }

    void LinePattern::UpdateLine()
    {
        Vector2d v = point2 - point1;
        double lng = v.Length();
        if (lng <= 0) {
            rectangle.SetSize(0, 0);
            return;
        }

        rectangle.SetSize(lng, width);
        rectangle.ClearTransform();
        rectangle.Translate((point2 + point1) * 0.5);
        rectangle.Rotate(-v.Angle(Vector2d(1.0, 0.0)));
    }

    void LinePattern::SetPattern(std::shared_ptr<Pattern> p)
    {
        rectangle.SetPattern(p);
    }

    std::shared_ptr<Pattern> LinePattern::GetPattern()
    {
        return rectangle.GetPattern();
    }

    bool LinePattern::HasPattern() const
    {
        return rectangle.HasPattern();
    }

} // end of namespace