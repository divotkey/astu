/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <cmath>
#include "Color.h"

namespace astu {    


    // double Color::Distance(const Color & o) const
    // {
    //     return sqrt( DistanceSquared(o) );        
    // }

    // double Color::DistanceSquared(const Color & o) const
    // {
    //     return (r - o.r) * (r - o.r) + (g - o.g) * (g - o.g) + (b - o.b) * (b - o.b) + (a - o.a) * (a - o.a);
    // }

    // Color & Color::MultiplyWithoutAlpha(double s) noexcept
    // {
    //     r *= s;
    //     g *= s;
    //     b *= s;
    //     return *this;
    // }

    // int Color::GetARGB() const
    // {
    //     return  ((int)(a * 255) << 24)
	// 			| ((int)(r * 255) << 16)
	// 			| ((int)(g * 255) << 8)
	// 			| ((int)(b * 255));
    // }

    // int Color::GetABGR() const
    // {
    //     return  ((int)(a * 255) << 24)
	// 			| ((int)(b * 255) << 16)
	// 			| ((int)(g * 255) << 8)
	// 			| ((int)(r * 255));
    // }

    Color & Color::Saturate() noexcept
    // {
    //     if (r > 1) {
    //         r = 1;
    //     } else if (r < 0) {
    //         r = 0;
    //     }

    //     if (g > 1) {
    //         g = 1;
    //     } else if (g < 0) {
    //         g = 0;
    //     }

    //     if (b > 1) {
    //         b = 1;
    //     } else if (b < 0) {
    //         b = 0;
    //     }

    //     if (a > 1) {
    //         a = 1;
    //     } else if (a < 0) {
    //         a = 0;
    //     }

    //     return *this;
    // }

    // Color & Color::Blend(const Color & o)
    // {
    //     double iba = 1.0 - o.a;
	// 	a = o.a + a * iba;
		
	// 	r = o.r * o.a + r * a * iba;
	// 	g = o.g * o.a + g * a * iba;
	// 	b = o.b * o.a + b * a * iba;
		
	// 	r /= a;
	// 	g /= a;
	// 	b /= a;

    //     return *this;
    // }

    // Color Color::Lerp(const Color & o, double t) const
    // {
    //     return *this + (o - *this) * t;
    // }


    // Color Color::operator+(const Color & o) const
    // {
    //     return Color(r + o.r, g + o.g, b + o.b, a + o.a);
    // }

    // Color& Color::operator+=(const Color & o)
    // {
    //     r += o.r;
    //     g += o.g;
    //     b += o.b;
    //     a += o.a;
    //     return *this;
    // }

    // Color Color::operator-(const Color & o) const
    // {
    //     return Color(r - o.r, g - o.g, b - o.b, a - o.a);
    // }

    // Color& Color::operator-=(const Color & o)
    // {
    //     r -= o.r;
    //     g -= o.g;
    //     b -= o.b;
    //     a -= o.a;
    //     return *this;
    // }

    // Color Color::operator*(double s) const
    // {
    //     return Color(r * s, g * s, b * s, a * s);
    // }

    // Color & Color::operator*=(double s)    
    // {
    //     r *= s;
    //     g *= s;
    //     b *= s;
    //     a *= s;
    //     return *this;
    // }

    // Color Color::operator/(double s) const
    // {
    //     return Color(r / s, g / s, b / s, a / s);
    // }

    // Color & Color::operator/=(double s)    
    // {
    //     r /= s;
    //     g /= s;
    //     b /= s;
    //     a /= s;
    //     return *this;
    // }    

    // bool Color::operator<(const Color & rhs) const
    // {
    //     //XXX this implementation is slow, improvement required.
    //     double lng1 = sqrt(r * r + g * g + b * b + a * a);
    //     double lng2 = sqrt(rhs.r * rhs.r + rhs.g * rhs.g + rhs.b * rhs.b + rhs.a * rhs.a);
    //     return lng1 < lng2;
    // }

    // bool Color::operator==(const Color& o) const
    // {
    //     return r == o.r && g == o.g && b == o.b && a == o.a;
    // }

} // end of namespace

// std::ostream& operator<<(std::ostream& os, const astu::Color& c)
// {
//     os << '{' << c.r << ", " << c.g << ", " << c.b << ", " << c.a << '}';
//     return os;
// }