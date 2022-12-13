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

// C++ Standard Library includes
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

namespace astu {

    void SimplePatternRenderer::Render(const Pattern & pattern, Image & result)
    {
        double dx = 1.0;
        double startX = dx / 2.0;
        double dy = 1.0;
        double startY = dy / 2.0;

        Vector2<double> p(0, startY);
        Color4d c1 = Color4d(0, 0, 0);
        Color4d c2 = Color4d(1, 1, 1);

        //cout << " 0 %";
        for (int j = 0; j < result.GetHeight(); ++j) {
            p.x = startX;
            for (int i = 0; i < result.GetWidth(); ++i) {
                Color4d c;
                if (pattern.GetColor(p, c)) {
                    c.Saturate();
                    result.SetPixel(i, j, c);
                }
                p.x += dx;
            }
            p.y += dy;

            //std::cout << "\b" << "\b" << "\b" << "\b";
            //std::cout << std::setw(2) << static_cast<int>((static_cast<double>(j) / result.GetHeight()) * 100) << " %";
            //std::cout.flush();
        }
        std::cout << endl;
    }

} // end of namespace