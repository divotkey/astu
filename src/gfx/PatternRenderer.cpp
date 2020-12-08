/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <iostream>
#include "Color.h"
#include "Image.h"
#include "Pattern.h"
#include "PatternRenderer.h"

using namespace std;

namespace astu {

    const int KERNEL_SIZE  = 5;
    const double KERNEL_RADIUS = 1.0;
    double kernel[KERNEL_SIZE * KERNEL_SIZE] {
        0.003765, 0.015019, 0.023792, 0.015019, 0.003765, 
        0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
        0.023792, 0.094907, 0.150342, 0.094907, 0.023792, 
        0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
        0.003765, 0.015019, 0.023792, 0.015019, 0.003765
    };

    // const int KERNEL_SIZE  = 3;
    // const double KERNEL_RADIUS = 1.0;
    // double kernel[KERNEL_SIZE * KERNEL_SIZE] {
    //     0.024879,	0.107973,	0.024879,
    //     0.107973,	0.468592,	0.107973,
    //     0.024879,	0.107973,	0.024879
    // };

    void SimplePatternRenderer::Render(const Pattern & pattern, Image & result)
    {
        double dx = 1.0;
        double startX = dx / 2.0;
        double dy = 1.0;
        double startY = dy / 2.0;

        Vector2<double> p(0, startY);
        Color c1 = Color(0, 0, 0);
        Color c2 = Color(1, 1, 1);
        for (int j = 0; j < result.GetHeight(); ++j) {
            p.x = startX;
            for (int i = 0; i < result.GetWidth(); ++i) {
                Color c;
                if (pattern.GetColor(p, c)) {
                    result.SetPixel(i, j, c);
                }
                p.x += dx;
            }
            // cout << endl;
            p.y += dy;
        }

    }

    void AntiAlisaingPatternRenderer::Render(const Pattern & pattern, Image & result)
    {
        double dx = 1.0;
        double startX = dx / 2.0;
        double dy = 1.0;
        double startY = dy / 2.0;

        Vector2<double> p(0, startY);
        Color c1 = Color(0, 0, 0);
        Color c2 = Color(1, 1, 1);
        for (int j = 0; j < result.GetHeight(); ++j) {
            p.x = startX;
            for (int i = 0; i < result.GetWidth(); ++i) {
                result.SetPixel(i, j, CalcColor(p, pattern));
                p.x += dx;
            }
            p.y += dy;
        }
    }

    Color AntiAlisaingPatternRenderer::CalcColor(const Vector2<double> & p, const Pattern & pattern)
    {
        const double dx = (KERNEL_RADIUS * 2) / KERNEL_SIZE;
        double startX = p.x - KERNEL_RADIUS;
        double dy = (KERNEL_RADIUS * 2) / KERNEL_SIZE;
        double startY = p.y - KERNEL_RADIUS;

        const Color c1 = Color(1, 1, 1);
        const Color c2 = Color(0, 0, 0);

        Color c;
        Vector2<double> aa(0, startY);
        double *k = kernel;
        for (int j = 0; j < KERNEL_SIZE; ++j) {
            aa.x = startX;
            for (int i = 0; i < KERNEL_SIZE; ++i) {
                aa.x += dx;
                Color localColor;
                if (pattern.GetColor(aa, localColor)) {
                    c += localColor * (*k++);
                } else {
                    ++k;
                }
            }
            aa.y += dy;
        }

        c.Saturate();
        return c;        
    }
}