/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/PatternRenderer.h"
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

        cout << " 0 %";
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

            std::cout << "\b" << "\b" << "\b" << "\b";
            std::cout << std::setw(2) << static_cast<int>((static_cast<double>(j) / result.GetHeight()) * 100) << " %";
            std::cout.flush();
        }
        std::cout << endl;
    }

    /////////////////////////////////////////////////
    /////// AntiAlisaingPatternRenderer
    /////////////////////////////////////////////////

    const double* AntiAlisaingPatternRenderer::GetKernel(AntialiasingLevel level)
    {
        // size 3x3, sigma = 0.8
        static const double kernel3x3[] = {
            0.06292,	0.124998,	0.06292,
            0.124998,	0.248326,	0.124998,
            0.06292,	0.124998,	0.06292,
        };

        // size 5x5, sigma = 1.15
        static const double kernel5x5[] = {
            0.007004,	0.020338,	0.029004,	0.020338,	0.007004,
            0.020338,	0.05906,	0.084226,	0.05906,	0.020338,
            0.029004,	0.084226,	0.120116,	0.084226,	0.029004,
            0.020338,	0.05906,	0.084226,	0.05906,	0.020338,
            0.007004,	0.020338,	0.029004,	0.020338,	0.007004,
        };

        // size 7x7, sigma = 1.3
        static const double kernel7x7[] = {
            0.000569,	0.002332,	0.00543,	0.00719,	0.00543,	0.002332,	0.000569,
            0.002332,	0.009553,	0.022245,	0.02948,	0.022245,	0.009553,	0.002332,
            0.00543,	0.022245,	0.051799,	0.068647,	0.051799,	0.022245,	0.00543,
            0.007196,	0.02948,	0.068647,	0.090973,	0.068647,	0.02948,	0.007196,
            0.00543,	0.022245,	0.051799,	0.068647,	0.051799,	0.022245,	0.00543,
            0.002332,	0.009553,	0.022245,	0.02948,	0.022245,	0.009553,	0.002332,
            0.000569,	0.002332,	0.00543,	0.00719,	0.00543,	0.002332,	0.000569,
        };

        // size 9x9, sigma = 1.5
        static const double kernel9x9[] = {
            0.000072,	0.000323,	0.000944,	0.001794,	0.002222,	0.001794,	0.000944,	0.000323,	0.000072,
            0.000323,	0.00145,	0.004233,	0.008048,	0.00997,	0.008048,	0.004233,	0.00145,	0.000323,
            0.000944,	0.004233,	0.012358,	0.023496,	0.029106,	0.023496,	0.012358,	0.004233,	0.000944,
            0.001794,	0.008048,	0.023496,	0.044672,	0.055338,	0.044672,	0.023496,	0.008048,	0.001794,
            0.002222,	0.00997,	0.029106,	0.055338,	0.068552,	0.055338,	0.029106,	0.00997,	0.002222,
            0.001794,	0.008048,	0.023496,	0.044672,	0.055338,	0.044672,	0.023496,	0.008048,	0.001794,
            0.000944,	0.004233,	0.012358,	0.023496,	0.029106,	0.023496,	0.012358,	0.004233,	0.000944,
            0.000323,	0.00145,	0.004233,	0.008048,	0.00997,	0.008048,	0.004233,	0.00145,	0.000323,
            0.000072,	0.000323,	0.000944,	0.001794,	0.002222,	0.001794,	0.000944,	0.000323,	0.000072,
        };

        switch (level) {
            case AntialiasingLevel::Simple:
                return kernel3x3;

            case AntialiasingLevel::Good:
                return kernel5x5;

            case AntialiasingLevel::Beautiful:
                return kernel7x7;

            case AntialiasingLevel::Insane:
                return kernel9x9;

            default:
                throw std::logic_error("Unknown anti-aliasing level");
        }
    }

    double AntiAlisaingPatternRenderer::GetKernelRadius(AntialiasingLevel level)
    {
        switch (level) {
            case AntialiasingLevel::Simple:
                return 0.8;

            case AntialiasingLevel::Good:
                return 1.15;

            case AntialiasingLevel::Beautiful:
                return 1.3;

            case AntialiasingLevel::Insane:
                return 1.5;

            default:
                throw std::logic_error("Unknown anti-aliasing level");
        }
    }

    unsigned int AntiAlisaingPatternRenderer::GetKernelSize(AntialiasingLevel level)
    {
        switch (level) {
            case AntialiasingLevel::Simple:
                return 3;

            case AntialiasingLevel::Good:
                return 5;

            case AntialiasingLevel::Beautiful:
                return 7;

            case AntialiasingLevel::Insane:
                return 9;

            default:
                throw std::logic_error("Unknown anti-aliasing level");
        }
    }


    // size 3x3, radius = 1.0
    // const double AntiAlisaingPatternRenderer::kKernel3x3[] = {
    //     0.024879,	0.107973,	0.024879,
    //     0.107973,	0.468592,	0.107973,
    //     0.024879,	0.107973,	0.024879
    // };

    // size 3x3, radius = 0.8
    // const double AntiAlisaingPatternRenderer::kKernel3x3[] = {
    //     0.06292,	0.124998,	0.06292,
    //     0.124998,	0.248326,	0.124998,
    //     0.06292,	0.124998,	0.06292,
    // };

    // size 5x5, radius = 1.0
    // const double AntiAlisaingPatternRenderer::kKernel5x5[] = {
    //     0.003765, 0.015019, 0.023792, 0.015019, 0.003765, 
    //     0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
    //     0.023792, 0.094907, 0.150342, 0.094907, 0.023792, 
    //     0.015019, 0.059912, 0.094907, 0.059912,	0.015019,
    //     0.003765, 0.015019, 0.023792, 0.015019, 0.003765
    // };

    // size 5x5, radius = 1.15
    // const double AntiAlisaingPatternRenderer::kKernel5x5[] = {
    //     0.007004,	0.020338,	0.029004,	0.020338,	0.007004,
    //     0.020338,	0.05906,	0.084226,	0.05906,	0.020338,
    //     0.029004,	0.084226,	0.120116,	0.084226,	0.029004,
    //     0.020338,	0.05906,	0.084226,	0.05906,	0.020338,
    //     0.007004,	0.020338,	0.029004,	0.020338,	0.007004,
    // };

    // size 7x7, radius = 1.5
    // const double AntiAlisaingPatternRenderer::kKernel7x7[] = {
    //     0.001500, 0.004380, 0.008328, 0.010317,	0.008328, 0.004380, 0.001500, 
    //     0.004380, 0.012788,	0.024314, 0.030120,	0.024314, 0.012788,	0.004380,
    //     0.008328, 0.024314,	0.046228, 0.057266,	0.046228, 0.024314,	0.008328,
    //     0.010317, 0.030120,	0.057266, 0.070940,	0.057266, 0.030120,	0.010317,
    //     0.008328, 0.024314,	0.046228, 0.057266,	0.046228, 0.024314,	0.008328,
    //     0.004380, 0.012788,	0.024314, 0.030120,	0.024314, 0.012788,	0.004380,
    //     0.001500, 0.004380, 0.008328, 0.010317,	0.008328, 0.004380, 0.001500
    // };

    // size 7x7, radius = 1.4
    // const double AntiAlisaingPatternRenderer::kKernel7x7[] = {
    //     0.000977,	0.00332,	0.006914,	0.008829,	0.006914,	0.00332,	0.000977,
    //     0.00332,	0.011286,	0.023505,	0.030014,	0.023505,	0.011286,	0.00332,
    //     0.006914,	0.023505,	0.048952,	0.062509,	0.048952,	0.023505,	0.006914,
    //     0.008829,	0.030014,	0.062509,	0.07982,	0.062509,	0.030014,	0.008829,
    //     0.006914,	0.023505,	0.048952,	0.062509,	0.048952,	0.023505,	0.006914,
    //     0.00332,	0.011286,	0.023505,	0.030014,	0.023505,	0.011286,	0.00332,
    //     0.000977,	0.00332,	0.006914,	0.008829,	0.006914,	0.00332,	0.000977,
    // };

    // size 7x7, radius = 1.3
    // const double AntiAlisaingPatternRenderer::kKernel7x7[] = {
    //     0.000569,	0.002332,	0.00543,	0.00719,	0.00543,	0.002332,	0.000569,
    //     0.002332,	0.009553,	0.022245,	0.02948,	0.022245,	0.009553,	0.002332,
    //     0.00543,	0.022245,	0.051799,	0.068647,	0.051799,	0.022245,	0.00543,
    //     0.007196,	0.02948,	0.068647,	0.090973,	0.068647,	0.02948,	0.007196,
    //     0.00543,	0.022245,	0.051799,	0.068647,	0.051799,	0.022245,	0.00543,
    //     0.002332,	0.009553,	0.022245,	0.02948,	0.022245,	0.009553,	0.002332,
    //     0.000569,	0.002332,	0.00543,	0.00719,	0.00543,	0.002332,	0.000569,
    // };

    // // size 7x7, radius = 1.2
    // const double AntiAlisaingPatternRenderer::kKernel7x7[] = {
    //     0.000286,	0.001476,	0.003949,	0.00548,	0.003949,	0.001476,	0.000286,
    //     0.001476,	0.00763,	0.020408,	0.02832,	0.020408,	0.00763,	0.001476,
    //     0.003949,	0.020408,	0.054587,	0.075751,	0.054587,	0.020408,	0.003949,
    //     0.00548,	0.02832,	0.075751,	0.105122,	0.075751,	0.02832,	0.00548,
    //     0.003949,	0.020408,	0.054587,	0.075751,	0.054587,	0.020408,	0.003949,
    //     0.001476,	0.00763,	0.020408,	0.02832,	0.020408,	0.00763,	0.001476,
    //     0.000286,	0.001476,	0.003949,	0.00548,	0.003949,	0.001476,	0.000286,
    // };

    // // size 7x7, radius = 1.2
    // const double AntiAlisaingPatternRenderer::kKernel7x7[] = {
    //     0.000286,	0.001476,	0.003949,	0.00548,	0.003949,	0.001476,	0.000286,
    //     0.001476,	0.00763,	0.020408,	0.02832,	0.020408,	0.00763,	0.001476,
    //     0.003949,	0.020408,	0.054587,	0.075751,	0.054587,	0.020408,	0.003949,
    //     0.00548,	0.02832,	0.075751,	0.105122,	0.075751,	0.02832,	0.00548,
    //     0.003949,	0.020408,	0.054587,	0.075751,	0.054587,	0.020408,	0.003949,
    //     0.001476,	0.00763,	0.020408,	0.02832,	0.020408,	0.00763,	0.001476,
    //     0.000286,	0.001476,	0.003949,	0.00548,	0.003949,	0.001476,	0.000286,
    // };

    // size 9x9, radius = 1.5
    // const double AntiAlisaingPatternRenderer::kKernel9x9[] = {
    //     0.000072,	0.000323,	0.000944,	0.001794,	0.002222,	0.001794,	0.000944,	0.000323,	0.000072,
    //     0.000323,	0.00145,	0.004233,	0.008048,	0.00997,	0.008048,	0.004233,	0.00145,	0.000323,
    //     0.000944,	0.004233,	0.012358,	0.023496,	0.029106,	0.023496,	0.012358,	0.004233,	0.000944,
    //     0.001794,	0.008048,	0.023496,	0.044672,	0.055338,	0.044672,	0.023496,	0.008048,	0.001794,
    //     0.002222,	0.00997,	0.029106,	0.055338,	0.068552,	0.055338,	0.029106,	0.00997,	0.002222,
    //     0.001794,	0.008048,	0.023496,	0.044672,	0.055338,	0.044672,	0.023496,	0.008048,	0.001794,
    //     0.000944,	0.004233,	0.012358,	0.023496,	0.029106,	0.023496,	0.012358,	0.004233,	0.000944,
    //     0.000323,	0.00145,	0.004233,	0.008048,	0.00997,	0.008048,	0.004233,	0.00145,	0.000323,
    //     0.000072,	0.000323,	0.000944,	0.001794,	0.002222,	0.001794,	0.000944,	0.000323,	0.000072,
    // };

    // const map<AntialiasingLevel, const double *> AntiAlisaingPatternRenderer::kLevelToKernel = {
    //         {AntialiasingLevel::Simple, kKernel3x3},
    //         {AntialiasingLevel::Good, kKernel5x5},
    //         {AntialiasingLevel::Beautiful, kKernel7x7},
    //         {AntialiasingLevel::Insane, kKernel9x9},
    //     };

    // const map<AntialiasingLevel, unsigned int> AntiAlisaingPatternRenderer::kLevelToSize = {
    //         {AntialiasingLevel::Simple, 3},
    //         {AntialiasingLevel::Good, 5},
    //         {AntialiasingLevel::Beautiful, 7},
    //         {AntialiasingLevel::Insane, 9}
    //     };

    // const map<AntialiasingLevel, double> AntiAlisaingPatternRenderer::kLevelToRadius = {
    //         {AntialiasingLevel::Simple, 0.8},
    //         {AntialiasingLevel::Good, 1.15},
    //         {AntialiasingLevel::Beautiful, 1.3},
    //         {AntialiasingLevel::Insane, 1.5}
    //     };

    AntiAlisaingPatternRenderer::AntiAlisaingPatternRenderer(AntialiasingLevel aaLevel)
        : kKernelRadius(GetKernelRadius(aaLevel))
        , kKernelSize(GetKernelSize(aaLevel))
        , kernel(GetKernel(aaLevel))
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

    Color4d AntiAlisaingPatternRenderer::CalcColor(const Vector2<double> & p, const Pattern & pattern)
    {
        const double dx = (kKernelRadius * 2) / kKernelSize;
        double startX = p.x - kKernelRadius;
        double dy = (kKernelRadius * 2) / kKernelSize;
        double startY = p.y - kKernelRadius;

        Color4d c(0, 0, 0, 0);

        Vector2<double> aa(0, startY);
        const double *k = kernel;
        for (unsigned int j = 0; j < kKernelSize; ++j) {
            aa.x = startX;
            for (unsigned int i = 0; i < kKernelSize; ++i) {
                aa.x += dx;
                Color4d localColor;
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