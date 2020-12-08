/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <string>
#include <cmath>
#include "Pattern.h"

namespace astu {

    Pattern::Pattern()
        : dirty(true)
    {
        // Intentionally left empty.
    }

    void Pattern::Translate(double x, double y)
    {
        transform.Translate(-x, -y);
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

    bool Pattern::GetColor(const Vector2<double> &p, Color & c) const
    {
        return GetColorTransformed(transform.TransformPoint(p), c);
    }

    BoundingBox Pattern::GetBoundingBox() const
    {
        if (dirty) {
            boundingBox = GetLocalBoundingBox();
            Matrix3 invTx = transform;
            invTx.Invert();
            boundingBox.Transform(invTx);
            dirty = false;
        }
        return boundingBox;
    }

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

    bool CirclePattern::GetColorTransformed(const Vector2<double> &pt, Color & c) const
    {
        if (pt.LengthSquared() > radiusSquared) {
            return false;
        }

        if (pattern) {
            return pattern->GetColor(pt, c);
        }
        return false;
    }

    BoundingBox CirclePattern::GetLocalBoundingBox() const 
    {
        return BoundingBox(radius * 2, radius * 2);
    }

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

    bool RectanglePattern::GetColorTransformed(const Vector2<double> &pt, Color & c) const
    {
        if (pt.x > hRadius || pt.x < -hRadius) {
            return false;
        }

        if (pt.y > vRadius || pt.y < -vRadius) {
            return false;
        }

        if (pattern) {
            return pattern->GetColor(pt, c);
        }
        
        return false;
    }

    BoundingBox RectanglePattern::GetLocalBoundingBox() const 
    {
        return BoundingBox(width, height);
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

    bool UnionPattern::GetColorTransformed(const Vector2<double> &pt, Color & c) const
    {
        bool hasColor = false;
        for (auto pattern : children) {
            Color localColor;
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

    void UnionPattern::Blend(Color & a, const Color & b) const
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