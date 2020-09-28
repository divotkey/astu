/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

namespace astu {
    
    class Color {
    public:
        // Color constants.
        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;

        static const Color Gray;
        static const Color Silver;
        static const Color SlateGray;
        static const Color LightGray;

        /** The red color component. */
        double r;

        /** The green color component. */
        double g;

        /** The blue color component. */
        double b;

        /** The alpha color component. */
        double a;

        static Color CreateFromRgb(int red, int green, int blue, int alpha = 255);

        /**
         * Constructor.
         * 
         * @param red   the red color component
         * @param green the green color component
         * @param blue  the blue color component
         * @param alpha the alpha color component
         */
        Color(double red = 0, double green = 0, double blue = 0, double alpha = 1);

        /**
         * Assigns a color using RGB values within the range [0, 1].
         * 
         * @param red   the red color component
         * @param green the green color component
         * @param blue  the blue color component
         * @param alpha the alpha color component
         */
        void Set(int red, int green, int blue, int alpha = 255);

        /**
         * Assigns a color using RGB values within the range [0, 1].
         * 
         * @param red   the red color component
         * @param green the green color component
         * @param blue  the blue color component
         * @param alpha the alpha color component
         */
        void Set(double red, double green, double blue, double alpha = 1);

        /**
         * Calculates the Euclidean in RGB color space.
         * 
         * @param o the other color
         * @return the distance to the other color
         */
        double DistanceWithoutAlpha(const Color & o) const;

        /**
         * Calculates the Euclidean squared in RGB color space.
         * 
         * @param o the other color
         * @return the distance to the other color
         */
        double DistanceSquaredWithoutAlpha(const Color & o) const;

        /**
         * Calculates the Euclidean in RGBA color space.
         * 
         * @param o the other color
         * @return the distance to the other color
         */
        double Distance(const Color & o) const;

        /**
         * Calculates the Euclidean squared in RGBA color space.
         * 
         * @param o the other color
         * @return the distance to the other color
         */
        double DistanceSquared(const Color & o) const;

        /**
         * Does a linear interpolation between this and the specified color.
         * 
         * @param o the other color
         * @param t the interpolation position in the inerval [0, 1]
         * @return  the new interpolated color
         */
        Color Lerp(const Color & o, double t) const;

        int GetARGB() const;

        /**
         * Clamps all color components within the range of 0 to 1.
         */
        void Saturate();

        Color & Blend(const Color & o);

        Color operator+(const Color & o) const;
        Color& operator+=(const Color & o);
        Color operator-(const Color & o) const;
        Color operator*(double s) const;
        Color & operator*=(double s);
        Color operator/(double s) const;
        Color & operator/=(double s);

		bool operator==(const Color& o) const {
			return r == o.r && g == o.g && b == o.b && a == o.a;
		}       

        bool operator!=(const Color& o) const {
            return !(*this == o);
        }         

        bool operator<(const Color & rhs) const;
    };
}