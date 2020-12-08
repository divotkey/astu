/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "Vector2.h"

namespace astu {

    class Pattern;
    class Image;

    class IPatternRenderer {
    public:
        virtual ~IPatternRenderer() {}
        virtual void Render(const Pattern & pattern, Image & result) = 0;
    };

    class SimplePatternRenderer : public IPatternRenderer {
    public:
        virtual void Render(const Pattern & pattern, Image & result) override;
    };

    class AntiAlisaingPatternRenderer : public IPatternRenderer {
    public:
        virtual void Render(const Pattern & pattern, Image & result) override;        

    private:
        Color CalcColor(const Vector2<double> & p, const Pattern & shape);
    };

    class StochasticAntiAlisaingPatternRenderer : public IPatternRenderer {
    public:
        virtual void Render(const Pattern & pattern, Image & result) override;        

    private:
        Color CalcColor(const Vector2<double> & p, const Pattern & shape);
    };

}