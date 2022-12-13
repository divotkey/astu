/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Graphics/IPatternRenderer.h"
#include "Graphics/Color.h"
#include "Graphics/RenderQuality.h"

// C++ Standard Library includes
#include <map>

namespace astu {

    class SimplePatternRenderer final : public IPatternRenderer {
    public:

        // Inherited via IPatternRenderer
        virtual void Render(const Pattern & pattern, Image & result) override;
    };

    class AntiAliasingPatternRenderer final : public IPatternRenderer {
    public:

        /** Used to map aa levels to kernels. */
        // static const std::map<AntialiasingLevel, const double *> kLevelToKernel;

        /** Used to map aa levels to kernel sizes. */
        // static const std::map<AntialiasingLevel, unsigned int> kLevelToSize;

        /** Used to map aa levels to kernel sizes. */
        // static const std::map<AntialiasingLevel, double> kLevelToRadius;

        /**
         * Constructor.
         * 
         * @param quality   defines the level of anti-aliasing
         */
        AntiAliasingPatternRenderer(AntialiasingLevel aaLevel = AntialiasingLevel::Good);

        // Inherited via IPatternRenderer
        virtual void Render(const Pattern & pattern, Image & result) override;        

    private:

        /** A 3x3 kernel used for 'simple' anti-aliasing. */
        static const double kKernel3x3[];

        /** A 5x5 kernel used for 'good' anti-aliasing. */
        static const double kKernel5x5[];

        /** A 5x5 kernel used for 'beautiful' anti-aliasing. */
        static const double kKernel7x7[];

        /** A 9x9 kernel used for 'insane' anti-aliasing. */
        static const double kKernel9x9[];

        /** The radius used for oversampling. */
        const double kKernelRadius;

        /** The size of the kernel (in one dimension). */
        const unsigned int kKernelSize;

        /** The kernel data. */
        double const * const kernel;

        static const double* GetKernel(AntialiasingLevel level);
        static unsigned int GetKernelSize(AntialiasingLevel level);
        static double GetKernelRadius(AntialiasingLevel level);

        // static unsigned int QualityToRadius(AntialiasingLevel level);

        Color4d CalcColor(const Vector2<double> & p, const Pattern & shape);
    };

    class StochasticAntiAliasingPatternRenderer : public IPatternRenderer {
    public:

        // Inherited via IPatternRenderer
        virtual void Render(const Pattern & pattern, Image & result) override;        

    private:
        Color4d CalcColor(const Vector2<double> & p, const Pattern & shape);
    };

}