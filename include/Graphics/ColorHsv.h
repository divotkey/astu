/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Color.h"

// C++ Standard Library includes
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cmath>

namespace astu {

    /**
     * Represents a color value in HSV (HSB) color space. 
     * 
     * A color is described using four color channels: hue, saturation and value (brightness).
     * 
     * @ingroup gfx_group
     */
    class ColorHsv final {
    public:
        /** The hue component of this color. */
        double h;

        /** The saturation component of this color. */
        double s;

        /** The brightness component of this color. */
        double v;

        /**
         * Constructor.
         * 
         * @param h the hue component of this color
         * @param s the saturation component of this color
         * @param v the brightness component of this color
         */
        ColorHsv(double h = 0, double s = 0, double v = 0)
            : h(h), s(s), v(v)
        {
            // Intentionally left empty.
        }

        /**
         * Constructor.
         * 
         * @param c the rgb color to initialize this color
         */
        explicit ColorHsv(const Color4d & c)
        {
            Set(c);
        }

        /**
         * Sets the components of this color.
         * 
         * @param h the hue component of this color
         * @param s the saturation component of this color
         * @param v the brightness component of this color
         */
        ColorHsv & Set(double h, double s, double v) {
            this->h = h;
            this->s = s;
            this->v = v;
            
            return *this;
        }

        /**
         * Sets this color to the specified color.
         * 
         * @param c the color in RGB color space
         */
        ColorHsv & Set(const Color4d & c) {
            double min = std::min({c.r, c.g, c.b});
            double max = std::max({c.r, c.g, c.b});
            double delta = max - min;

            this->v = max;
            if (max != 0) {
                this->s = delta / max;
            }
            else {                           
                // r = g = b = 0
                this->s = 0; 
                this->h = -1; 
                return *this;
            }

            if (max == min) {                
                // Everything is gray.
                this->h = 0; 
                this->s = 0;
                return *this;
            }

            if (c.r == max) {
                // Between Yellow and  Magenta.
                this->h = (c.g - c.b ) / delta;       
            }
            else if(c.g == max ) {
                // Between Cyan and Yellow.
                this->h = 2 + (c.b - c.r) / delta;   
            }
            else {
                // Between Magenta and Cyan
                this->h = 4.0 + (c.r - c.g) / delta;   
            }

            // Make Degrees.
            this->h *= 60;                     

            if( this->h < 0 ) {
                this->h += 360;
            }

            return *this;
        }

        /**
         * Converts this HSV color to an RGB color.
         * 
         * @return the corresponding RGB color.
         */
        Color4d ToRgb() const {
            int i;
            if (s == 0) { 
                // Achromatic (Gray).
                return Color4d(v, v, v);
            }

            // Sector 0 to 5.
            double h = this->h / 60.0;           

            i = static_cast<int>(floor(h));

            // Factorial part of h.
            double f = h - i;         

            double p = v * (1.0 - s );
            double q = v * (1.0 - s * f);
            double t = v * ( 1 - s * ( 1 - f ) );

            switch( i ) {
                case 0: 
                    // *r = v; *g = t; *b = p; 
                    return Color4d(v, t, p);

                case 1:
                    //*r = q; *g = v; *b = p;
                    return Color4d(q, v, p);

                case 2: 
                    // *r = p; *g = v; *b = t; 
                    return Color4d(p, v, t);

                case 3:
                    // *r = p; *g = q; *b = v;
                    return Color4d(p, q, v);

                case 4: 
                    // *r = t; *g = p; *b = v;
                    return Color4d(t, p, v);

                case 5:
                    // *r = v; *g = p; *b = q; 
                    return Color4d(v, p, q);

                default:
                    // Should never happen.
                    throw std::logic_error("Conversion of HSV color to RGB failed");
            }        
        }

    };


} // end of namespace

/**
 * Insertion operator for color classes.
 * 
 * @param os    the output stream
 * @param c     the color
 * @return  reference to the output stream
 */
inline std::ostream& operator<<(std::ostream& os, const astu::ColorHsv& c)
{
    os << '{' << c.h << ", " << c.s << ", " << c.v << '}';
    return os;
}