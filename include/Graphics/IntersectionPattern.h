/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/CompoundPattern.h"

namespace astu {

    class IntersectionPattern : public CompoundPattern {
    public:

        /** Constructor. */
        IntersectionPattern() = default;

    protected:
        // Inherited via CompoundPattern
        bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        BoundingBox GetLocalBoundingBox() const override;
        bool IsInsideTransformed(const Vector2<double> &pt) const override;

    private:
        //void Blend(Color4d & a, const Color4d & b) const;
    };

} // end of namespace