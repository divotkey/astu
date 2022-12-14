/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/SimplePatternRenderer.h"
#include "Graphics/Color.h"
#include "Graphics/Image.h"
#include "Graphics/Pattern.h"

using namespace std;

namespace astu {

    void SimplePatternRenderer::DoRender(const Pattern & pattern, Image & result)
    {
        double dx = 1.0;
        double startX = dx / 2.0;
        double dy = 1.0;
        double startY = dy / 2.0;

        Vector2<double> p(0, startY);

        for (int j = 0; j < result.GetHeight(); ++j) {
            p.x = startX;
            for (int i = 0; i < result.GetWidth(); ++i) {
                Color4d c;
                if (pattern.GetColor(transform.TransformPoint(p), c)) {
                    c.Saturate();
                    result.AddPixel(i, j, c);
                }
                p.x += dx;
            }
            p.y += dy;
        }
    }

} // end of namespace