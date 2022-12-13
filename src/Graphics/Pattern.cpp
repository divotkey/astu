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

} // end of namespace