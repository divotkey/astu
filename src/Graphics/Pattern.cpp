/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/Pattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    /////////////////////////////////////////////////
    /////// Class Pattern
    /////////////////////////////////////////////////

    Pattern::Pattern() : dirty(true)
    {
        // Intentionally left empty.
    }

    void Pattern::Translate(double tx, double ty)
    {
        transform.Translate(-tx, -ty);
        dirty = true;
    }

    void Pattern::Scale(double sx, double sy)
    {
        if (sx == 0 || sy == 0) {
            throw std::domain_error("Scaling factors must not be zero");
        }
        transform.Scale(1.0 / sx, 1.0 / sy);
        dirty = true;
    }

    void Pattern::Rotate(double phi) 
    {
        transform.Rotate(-phi);
        dirty = true;
    }

    bool Pattern::GetColor(const Vector2<double> &p, Color4d & outColor) const
    {
        return GetColorTransformed(transform.TransformPoint(p), outColor);
    }

    BoundingBox Pattern::GetBoundingBox() const
    {
        if (dirty) {
            boundingBox = GetLocalBoundingBox();
            Matrix3d invTx = transform;
            invTx.Invert();
            boundingBox.Transform(invTx);
            dirty = false;
        }
        return boundingBox;
    }

    /////////////////////////////////////////////////
    /////// Class UnicolorPattern
    /////////////////////////////////////////////////

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


    /////////////////////////////////////////////////
    /////// Class RectanglePattern
    /////////////////////////////////////////////////

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


    /////////////////////////////////////////////////
    /////// Class CirclePattern
    /////////////////////////////////////////////////

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


    /////////////////////////////////////////////////
    /////// Compound Pattern
    /////////////////////////////////////////////////

    void CompoundPattern::Add(std::shared_ptr<Pattern> child)
    {
        children.push_back(child);
        OnPatternAdded(*child);        
    }

    void CompoundPattern::Clear()
    {
        children.clear();
        OnClear();
    }

    /////////////////////////////////////////////////
    /////// Union Pattern
    /////////////////////////////////////////////////

    bool UnionPattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        bool hasColor = false;
        for (auto pattern : children) {
            Color4d localColor;
            if (pattern->GetColor(pt, localColor)) {
                if (hasColor) {
                    Blend(c, localColor);
                } else {
                    hasColor = true;
                    c = localColor;
                }
            }
        }

        return hasColor;
    }

    void UnionPattern::Blend(Color4d & a, const Color4d & b) const
    {
        double iba = 1.0 - b.a;
		a.a = b.a + a.a * iba;
		
		a.r = b.r * b.a + a.r * a.a * iba;
		a.g = b.g * b.a + a.g * a.a * iba;
		a.b = b.b * b.a + a.b * a.a * iba;
		
		a.r /= a.a;
		a.g /= a.a;
		a.b /= a.a;
    }

    BoundingBox UnionPattern::GetLocalBoundingBox() const
    {
        BoundingBox result;

        for (const auto pattern : children) {
            result.Merge(pattern->GetBoundingBox());
        }

        return result;
    }
}