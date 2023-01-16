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

using namespace std;

namespace astu {

    BlurPattern::BlurPattern(double r, unsigned int nSamples)
        : nMinSamples(4), threshold(0.00001)
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

    //bool BlurPattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    //{
    //    bool hit = false;
    //    c.Set(0, 0, 0, 0);
    //
    //    auto &random = Random::GetInstance();
    //    unsigned int nHits = 0;
    //
    //    for (unsigned int i = 0; i < nSamples; ++i) {
    //        Vector2d v(random.NextDouble(0, radius), 0);
    //        v.Rotate(random.NextDouble(0, MathUtils::PI2d));
    //
    //        Color4d col;
    //        if (UnionPattern::GetColorTransformed(pt + v, col)) {
    //            c += col;
    //            ++nHits;
    //        }
    //    }
    //
    //    if (nHits > 0) {
    //        unsigned int misses = nSamples - nHits;
    //        c /= nHits;
    //        c.a *= 1.0 - static_cast<double>(misses) / nSamples;
    //        return true;
    //    }
    //
    //    return false;
    //}

    bool BlurPattern::GetColorTransformed(const Vector2<double> &pt, Color4d & c) const
    {
        bool hit = false;
        c.Set(0, 0, 0, 0);

        auto &random = Random::GetInstance();
        unsigned int nHits = 0;
        unsigned int cntSamples = 0;

        for (unsigned int i = 0; i < nMinSamples - 1; ++i) {
            Vector2d v(random.NextDouble(0, radius), 0);
            v.Rotate(random.NextDouble(0, MathUtils::PI2d));

            Color4d col;
            if (UnionPattern::GetColorTransformed(pt + v, col)) {
                c += col;
                ++nHits;
            }
            ++cntSamples;
        }

        if (nHits == 0) {
            return false;
        }

        Color4d prevCol = c / nHits;
        unsigned int misses = cntSamples - nHits;
        prevCol.a *= 1.0 - static_cast<double>(misses) / cntSamples;
        //auto thSquared = threshold * threshold;
        for (unsigned int i = nMinSamples - 1; i < nSamples; ++i) {
            Vector2d v(random.NextDouble(0, radius), 0);
            v.Rotate(random.NextDouble(0, MathUtils::PI2d));

            Color4d col;
            if (UnionPattern::GetColorTransformed(pt + v, col)) {
                c += col;
                ++nHits;
            }
            ++cntSamples;

            misses = cntSamples - nHits;
            Color4d curCol = c / nHits;
            curCol.a *= 1.0 - static_cast<double>(misses) / cntSamples;

            auto dist = curCol.Distance(prevCol);
            if (dist < threshold) {
                c = curCol;
                return true;
            }
            prevCol = curCol;
        }

        c = prevCol;
        return true;
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

    void BlurPattern::SetNumMinSamples(unsigned int n)
    {
        nMinSamples = n;
    }

    unsigned int BlurPattern::GetNumMinSamples() const
    {
        return nMinSamples;
    }

    void BlurPattern::SetThreshold(double th)
    {
        if (th < 0) {
            throw std::domain_error("Threshold value must be greater equal zero, got " + to_string(th));
        }
        threshold = th;
    }

    double BlurPattern::GetThreshold() const
    {
        return threshold;
    }

} // end of namespace