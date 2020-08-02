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

        int GetARGB() const;

        /**
         * Clamps all color components within the range of 0 to 1.
         */
        void Saturate();

        Color & Blend(const Color & o);

        Color operator+(const Color & o) const;
        Color& operator+=(const Color & o);
        Color operator*(double s) const;
        Color & operator*=(double s);
        Color operator/(double s) const;
        Color & operator/=(double s);
    };
}