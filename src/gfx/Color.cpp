/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include "Color.h"

namespace astu {

    Color Color::CreateFromRgb(int red, int green, int blue, int alpha)
    {
        Color result;
        result.Set(red, green, blue, alpha);
        return result;
    }

    Color::Color(double red, double green, double blue, double alpha)
        : r(red)
        , g(green)
        , b(blue)
        , a(alpha)
    {
        // Intentionally left empty.
    }

    void Color::Set(int red, int green, int blue, int alpha)
    {
        r = red / 255.0;
        g = green / 255.0;
        b = blue / 255.0;
        a = alpha / 255.0;
    }

    void Color::Set(double red, double green, double blue, double alpha)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    int Color::GetARGB() const
    {
        return  ((int)(a * 255) << 24)
				| ((int)(r * 255) << 16)
				| ((int)(g * 255) << 8)
				| ((int)(b * 255));
    }

    void Color::Saturate()
    {
        if (r > 1) {
            r = 1;
        } else if (r < 0) {
            r = 0;
        }

        if (g > 1) {
            g = 1;
        } else if (g < 0) {
            g = 0;
        }

        if (b > 1) {
            b = 1;
        } else if (b < 0) {
            b = 0;
        }

        if (a > 1) {
            a = 1;
        } else if (a < 0) {
            a = 0;
        }
    }

    Color & Color::Blend(const Color & o)
    {
        double iba = 1.0 - o.a;
		a = o.a + a * iba;
		
		r = o.r * o.a + r * a * iba;
		g = o.g * o.a + g * a * iba;
		b = o.b * o.a + b * a * iba;
		
		r /= a;
		g /= a;
		b /= a;

        return *this;
    }

    Color Color::operator+(const Color & o) const
    {
        return Color(r + o.r, g + o.g, b + o.b, a + o.a);
    }

    Color& Color::operator+=(const Color & o)
    {
        r += o.r;
        g += o.g;
        b += o.b;
        a += o.a;
        return *this;
    }

    Color Color::operator*(double s) const
    {
        return Color(r * s, g * s, b * s, a * s);
    }

    Color & Color::operator*=(double s)    
    {
        r *= s;
        g *= s;
        b *= s;
        a *= s;
        return *this;
    }

    Color Color::operator/(double s) const
    {
        return Color(r / s, g / s, b / s, a / s);
    }

    Color & Color::operator/=(double s)    
    {
        r /= s;
        g /= s;
        b /= s;
        a /= s;
        return *this;
    }    
}