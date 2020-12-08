/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "Turtle.h"

namespace astu {

    const Vector2<double> Turtle::REF_DIRECTION(0, -1);

    Turtle::Turtle()
    {
        Reset();
    }

    void Turtle::Reset()
    {
        Go(0, 0);
        SetDirection(0);
        PenDown();
        SetPenColor(Color(1, 1, 1));
        SetPenWidth(2);
    }

    const Vector2<double> & Turtle::GetPosition() const
    {
        return position;
    }

    void Turtle::SetPenColor(const Color& c)
    {
        penColor = c;
    }

    const Color & Turtle::GetPenColor() const
    {
        return penColor;
    }

    void Turtle::Go(double x, double y)
    {
        position.Set(x, y);
    }

    void Turtle::Forward(double distance)
    {
        Vector2<double> dir(REF_DIRECTION);
        dir.Rotate(angle);
        position += dir * distance;
    }

    void Turtle::TurnLeft(double phi)
    {
        angle -= ToRadians(phi);
    }

    void Turtle::TurnRight(double phi)
    {
        angle += ToRadians(phi);
    }

    void Turtle::SetDirection(double phi)
    {
        angle = ToRadians(phi);
    }

    void Turtle::PenDown()
    {
        penDown = true;
    }

    void Turtle::PenUp()
    {
        penDown = false;
    }

    bool Turtle::IsPenDown()
    {
        return penDown;
    }

    void Turtle::SetPenWidth(double w)
    {
        if (w > 0) {
            penWidth = w;
        }
    }

    double Turtle::GetPenWidth() const
    {
        return penWidth;
    }

}