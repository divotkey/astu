/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
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

    void Pattern::TranslateGeometric(double tx, double ty)
    {
        transform *= Matrix3d::CreateTranslate(-tx, -ty);
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

    void Pattern::ScaleGeometric(double sx, double sy)
    {
        if (sx == 0 || sy == 0) {
            throw std::domain_error("Scaling factors must not be zero");
        }

        transform *= Matrix3d::CreateScale(1.0 / sx, 1.0 / sy);
        dirty = true;
    }

    void Pattern::Rotate(double phi) 
    {
        transform.Rotate(-phi);
        dirty = true;
    }

    void Pattern::RotateGeometric(double phi)
    {
        transform *= Matrix3d::CreateRotate(phi);
        dirty = true;
    }

    void Pattern::ClearTransform() {
        transform.SetToIdentity();
        dirty = true;
    }

    bool Pattern::GetColor(const Vector2<double> &p, Color4d & outColor) const
    {
        return GetColorTransformed(transform.TransformPoint(p), outColor);
    }

    bool Pattern::IsInside(const Vector2<double> &p) const
    {
        return IsInsideTransformed(transform.TransformPoint(p));
    }

    //bool Pattern::IsInside(const BoundingBox &bbox) const
    //{
    //    BoundingBox bboxT(bbox);
    //    bboxT.Transform(transform);
    //
    //    return IsInsideTransformed(bboxT);
    //}

    BoundingBox Pattern::GetBoundingBox() const
    {
        UpdateBoundingBox();
        return boundingBox;
    }

    void Pattern::Prepare()
    {
        UpdateBoundingBox();
    }

    void Pattern::UpdateBoundingBox() const
    {
        if (dirty) {
            boundingBox = GetLocalBoundingBox();
            Matrix3d invTx = transform;
            invTx.Invert();
            boundingBox.Transform(invTx);
            dirty = false;
        }
    }

} // end of namespace