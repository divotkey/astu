/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "AstUtils.h"
#include "Vector2.h"
#include "Color.h"

namespace astu {

    class Turtle {
    public:
        Turtle();

        void Reset();
        void Go(double x, double y);
        void Forward(double distance);
        void PenDown();
        void PenUp();
        void TurnLeft(double phi);
        void TurnRight(double phi);
        void SetDirection(double phi);
        void SetPenWidth(double w);
        double GetPenWidth() const;
        const Vector2<double> & GetPosition() const;

        bool IsPenDown();
        void SetPenColor(const Color& c);
        const Color & GetPenColor() const;

    private:
        static const Vector2<double> REF_DIRECTION;
        Vector2<double> position;
        double angle;
        Color penColor;
        double penWidth;
        bool penDown;
    };

}