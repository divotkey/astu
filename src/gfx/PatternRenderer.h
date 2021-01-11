/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <map>

#include "Color.h"
#include "Vector2.h"
#include "RenderQuality.h"

namespace astu {

    class Pattern;
    class Image;

    class IPatternRenderer {
    public:

        /**
         * Virtual destructor.
         */        
        virtual ~IPatternRenderer() {}

        virtual void Render(const Pattern & pattern, Image & result) = 0;
    };

    class SimplePatternRenderer final : public IPatternRenderer {
    public:

        // Inherited via IPatternRenderer
        virtual void Render(const Pattern & pattern, Image & result) override;
    };

    class AntiAlisaingPatternRenderer final : public IPatternRenderer {
    public:

        /** Used to map aa levels to kernels. */
        static const std::map<AntialiasingLevel, const double *> kLevelToKernel;

        /** Used to map aa levels to kernel sizes. */
        static const std::map<AntialiasingLevel, unsigned int> kLevelToSize;

        /** Used to map aa levels to kernel sizes. */
        static const std::map<AntialiasingLevel, double> kLevelToRadius;


        /**
         * Constructor.
         * 
         * @param quality   defines the level of anti-aliasing
         */
        AntiAlisaingPatternRenderer(AntialiasingLevel aaLevel = AntialiasingLevel::Good);

        // Inherited via IPatternRenderer
        virtual void Render(const Pattern & pattern, Image & result) override;        

    private:

        /** A 3x3 kernel used for 'simple' anti-aliasing. */
        static const double kKernel3x3[];

        /** A 5x5 kernel used for 'good' anti-aliasing. */
        static const double kKernel5x5[];

        /** A 5x5 kernel used for 'beautiful' anti-aliasing. */
        static const double kKernel7x7[];


        /** The radius used for oversampling. */
        const double kKernelRadius;

        /** The size of the kernel (in one dimension). */
        const unsigned int kKernelSize;

        /** The kernel data. */
        double const * const kernel;

        static unsigned int QualityToRadius(AntialiasingLevel level);


        Color CalcColor(const Vector2<double> & p, const Pattern & shape);
    };

    class StochasticAntiAlisaingPatternRenderer : public IPatternRenderer {
    public:

        // Inherited via IPatternRenderer
        virtual void Render(const Pattern & pattern, Image & result) override;        

    private:
        Color CalcColor(const Vector2<double> & p, const Pattern & shape);
    };

}