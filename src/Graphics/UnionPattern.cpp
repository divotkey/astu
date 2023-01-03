/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/UnionPattern.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    bool UnionPattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        bool hasColor = false;
        for (auto pattern : children) {
            Color4d localColor;
            if (pattern->GetColor(pt, localColor)) {
                if (hasColor) {
                    c.Blend(localColor);
                    //Blend(c, localColor);
                } else {
                    hasColor = true;
                    c = localColor;
                }
            }
        }

        return hasColor;
    }

    //void UnionPattern::Blend(Color4d & a, const Color4d & b) const
    //{
    //    double iba = 1.0 - b.a;
	//	a.a = b.a + a.a * iba;
	//
	//	a.r = b.r * b.a + a.r * a.a * iba;
	//	a.g = b.g * b.a + a.g * a.a * iba;
	//	a.b = b.b * b.a + a.b * a.a * iba;
	//
	//	a.r /= a.a;
	//	a.g /= a.a;
	//	a.b /= a.a;
    //}

    BoundingBox UnionPattern::GetLocalBoundingBox() const
    {
        BoundingBox result;

        for (const auto pattern : children) {
            result.Merge(pattern->GetBoundingBox());
        }

        return result;
    }

} // end of namespace