/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <iostream>
#include <iomanip>

#include "Color.h"
#include "Image.h"
#include "Pattern.h"
#include "PatternRenderer.h"

using namespace std;

namespace astu {

    // const int kKernelSize  = 7;
    // const double kKernelRadius = 1.5;
    // double kernel[kKernelSize * kKernelSize] {
    //     0.001500, 0.004380, 0.008328, 0.010317,	0.008328, 0.004380, 0.001500, 
    //     0.004380, 0.012788,	0.024314, 0.030120,	0.024314, 0.012788,	0.004380,
    //     0.008328, 0.024314,	0.046228, 0.057266,	0.046228, 0.024314,	0.008328,
    //     0.010317, 0.030120,	0.057266, 0.070940,	0.057266, 0.030120,	0.010317,
    //     0.008328, 0.024314,	0.046228, 0.057266,	0.046228, 0.024314,	0.008328,
    //     0.004380, 0.012788,	0.024314, 0.030120,	0.024314, 0.012788,	0.004380,
    //     0.001500, 0.004380, 0.008328, 0.010317,	0.008328, 0.004380, 0.001500
    // };

    // const int kKernelSize  = 5;
    // const double kKernelRadius = 1.0;
    // double kernel[kKernelSize * kKernelSize] {
    //     0.003765, 0.015019, 0.023792, 0.015019, 0.003765, 
    //     0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
    //     0.023792, 0.094907, 0.150342, 0.094907, 0.023792, 
    //     0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
    //     0.003765, 0.015019, 0.023792, 0.015019, 0.003765
    // };

    // const int kKernelSize  = 3;
    // const double kKernelRadius = 1.0;
    // double kernel[kKernelSize * kKernelSize] {
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

        cout << " 0 %";
        for (int j = 0; j < result.GetHeight(); ++j) {
            p.x = startX;
            for (int i = 0; i < result.GetWidth(); ++i) {
                Color c;
                if (pattern.GetColor(p, c)) {
                    c.Saturate();
                    result.SetPixel(i, j, c);
                }
                p.x += dx;
            }
            p.y += dy;

            std::cout << "\b" << "\b" << "\b" << "\b";
            std::cout << std::setw(2) << static_cast<int>((static_cast<double>(j) / result.GetHeight()) * 100) << " %";
            std::cout.flush();
        }
        std::cout << endl;
    }

    /////////////////////////////////////////////////
    /////// AntiAlisaingPatternRenderer
    /////////////////////////////////////////////////

    // size 3x3, radius = 1.0
    const double AntiAlisaingPatternRenderer::kKernel3x3[] = {
        0.024879,	0.107973,	0.024879,
        0.107973,	0.468592,	0.107973,
        0.024879,	0.107973,	0.024879
    };

    // size 5x5, radius = 1.0
    const double AntiAlisaingPatternRenderer::kKernel5x5[] = {
        0.003765, 0.015019, 0.023792, 0.015019, 0.003765, 
        0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
        0.023792, 0.094907, 0.150342, 0.094907, 0.023792, 
        0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
        0.003765, 0.015019, 0.023792, 0.015019, 0.003765
    };

    // size 7x7, radius = 1.5
    const double AntiAlisaingPatternRenderer::kKernel7x7[] = {
        0.001500, 0.004380, 0.008328, 0.010317,	0.008328, 0.004380, 0.001500, 
        0.004380, 0.012788,	0.024314, 0.030120,	0.024314, 0.012788,	0.004380,
        0.008328, 0.024314,	0.046228, 0.057266,	0.046228, 0.024314,	0.008328,
        0.010317, 0.030120,	0.057266, 0.070940,	0.057266, 0.030120,	0.010317,
        0.008328, 0.024314,	0.046228, 0.057266,	0.046228, 0.024314,	0.008328,
        0.004380, 0.012788,	0.024314, 0.030120,	0.024314, 0.012788,	0.004380,
        0.001500, 0.004380, 0.008328, 0.010317,	0.008328, 0.004380, 0.001500
    };

    const map<AntialiasingLevel, const double *> AntiAlisaingPatternRenderer::kLevelToKernel = {
            {AntialiasingLevel::Simple, kKernel3x3},
            {AntialiasingLevel::Good, kKernel5x5},
            {AntialiasingLevel::Beautiful, kKernel7x7},
            // {AntialiasingLevel::Beautiful, 7},
            // {AntialiasingLevel::Insane, 9}
        };

    const map<AntialiasingLevel, unsigned int> AntiAlisaingPatternRenderer::kLevelToSize = {
            {AntialiasingLevel::Simple, 3},
            {AntialiasingLevel::Good, 5},
            {AntialiasingLevel::Beautiful, 7},
            // {AntialiasingLevel::Insane, 9}
        };

    const map<AntialiasingLevel, double> AntiAlisaingPatternRenderer::kLevelToRadius = {
            {AntialiasingLevel::Simple, 1.0},
            {AntialiasingLevel::Good, 1.0},
            {AntialiasingLevel::Beautiful, 1.5},
            // {AntialiasingLevel::Insane, 1.5}
        };

    AntiAlisaingPatternRenderer::AntiAlisaingPatternRenderer(AntialiasingLevel aaLevel)
        : kKernelRadius(kLevelToRadius.at(aaLevel))
        , kKernelSize(kLevelToSize.at(aaLevel))
        , kernel(kLevelToKernel.at(aaLevel))
    {
        // Intentionally left empty.
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

        cout << " 0 %";
        for (int j = 0; j < result.GetHeight(); ++j) {
            p.x = startX;
            for (int i = 0; i < result.GetWidth(); ++i) {
                result.SetPixel(i, j, CalcColor(p, pattern));
                p.x += dx;
            }
            p.y += dy;

            std::cout << "\b" << "\b" << "\b" << "\b";
            std::cout << std::setw(2) << static_cast<int>((static_cast<double>(j) / result.GetHeight()) * 100) << " %";
            std::cout.flush();
        }
        std::cout << endl;
    }

    Color AntiAlisaingPatternRenderer::CalcColor(const Vector2<double> & p, const Pattern & pattern)
    {
        const double dx = (kKernelRadius * 2) / kKernelSize;
        double startX = p.x - kKernelRadius;
        double dy = (kKernelRadius * 2) / kKernelSize;
        double startY = p.y - kKernelRadius;

        const Color c1 = Color(1, 1, 1);
        const Color c2 = Color(0, 0, 0);

        Color c;
        Vector2<double> aa(0, startY);
        const double *k = kernel;
        for (unsigned int j = 0; j < kKernelSize; ++j) {
            aa.x = startX;
            for (unsigned int i = 0; i < kKernelSize; ++i) {
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