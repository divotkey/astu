/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/BlurPattern.h"
#include "Math/Random.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    BlurPattern::BlurPattern(double r, unsigned int nSamples)
    {
        SetRadius(r);
        SetNumSamples(nSamples);
    }

    void BlurPattern::SetRadius(double r)
    {
        if (r <= 0) {
            throw std::domain_error("Circle radius must be greater zero, got " + std::to_string(r));
        }
        radius = r;
    }

    bool BlurPattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        bool hit = false;
        c.Set(0, 0, 0, 0);

        auto &random = Random::GetInstance();
        unsigned int nHits = 0;

        for (unsigned int i = 0; i < nSamples; ++i) {
            Vector2d v(random.NextDouble(0, radius), 0);
            v.Rotate(random.NextDouble(0, MathUtils::PI2d));

            Color4d col;
            if (UnionPattern::GetColorTransformed(pt + v, col)) {
                c += col;
                ++nHits;
            }
        }

        if (nHits > 0) {
            unsigned int misses = nSamples - nHits;
            c /= nHits;
            c.a *= 1.0 - static_cast<double>(misses) / nSamples;
            //if (c.a != 1)
            //    std::cout << "c.a = " << c.a << std::endl;
            return true;
        }

        return false;

        return nHits > 0;
    }


    void BlurPattern::SetNumSamples(unsigned int n)
    {
        nSamples = n;
    }

    unsigned int BlurPattern::GetNumSamples() const
    {
        return nSamples;
    }

    BoundingBox BlurPattern::GetLocalBoundingBox() const
    {
        auto bb = UnionPattern::GetLocalBoundingBox();
        bb.SetWidth(bb.GetWidth() + radius * 2);
        bb.SetHeight(bb.GetHeight() + radius * 2);
        return bb;
    }

} // end of namespace