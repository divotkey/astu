/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <iostream>

namespace astu {
    
    /**
     * Represents a color value.
     * 
     * A color is described using four color channels: red, green, blue and
     * alpha. The alpha channel represents transparency. The color channels
     * are represents as floating-point values which normally lie within the
     * interval [0, 1].
     * 
     * @ingroup gfx_group
     */
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
         * Constructor.
         * 
         * This constructor can be used to create colors using the usual
         * hex-triplet notation which is a six-digit, three-byte hexadecimal
         * number used int HTML, CSS, SVG etc.
         * 
         * **Example**
         * 
         * ```
         * Color mediumAquamarine(0x66cdaa);
         * ```
         * 
         * The enum WebColors defines integer constants according to the
         * color names defined by the W3C standards. These constants can be used
         * to initialize colors using this constructor.
         * 
         * ```
         * Color redColor(WebColors::Red);
         * Color PurpleColor(WebColors::Purple);
         * ```
         * 
         * This constructor also functions as automatic conversion constructor
         * which allows C++ to automatically convert integers to objects of class
         * Color. This makes it particularly easy to use the WebColor constants 
         * to initialize colors:
         * 
         * ```
         * Color redColor = WebColors::Red;
         * ```
         * 
         * or even as function arguments of type color
         * 
         * ```
         * #include <Color.h>
         * 
         * void SomeFunction(const Color & c) {
         *  // Does something with c...
         * }
         * 
         * int main() {
         *   SomeFunction(WebColors::Aqua);
         * 
         *   return 0;
         * }
         * ```
         * 
         * @param rgb   the rgb color components encoded within in an integer
         */
        Color(int rgb);

        /**
         * Sets the alpha channel of this color.
         * 
         * @param a the alpha channel
         * @return a reference to this color for method chaining
         */
        Color & SetAlpha(double a) {
            this->a = a;
            return *this;
        }

        /**
         * Assigns a color using RGBA values within the range [0, 1].
         * 
         * @param red   the red color component
         * @param green the green color component
         * @param blue  the blue color component
         * @param alpha the alpha color component
         */
        void Set(double red, double green, double blue, double alpha = 1);

        /**
		 * Converts this color to an RGBA integer.
		 *
		 * @return the RGBA integer representing this color
		 */
        int ToRgba() const {
			return
				  (static_cast<int>(r * 255) << 24)
				| (static_cast<int>(g * 255) << 16)
				| (static_cast<int>(b * 255) << 8)
				| (static_cast<int>(a * 255));
        }

        /**
         * Calculates the Euclidean distance in RGB color space.
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
         * @param t the interpolation position in the interval [0, 1]
         * @return  the new interpolated color
         */
        Color Lerp(const Color & o, double t) const;

        /**
         * Converts this color to an integer value.
         * 
         * @return the integer representation of this color
         */
        int GetARGB() const;

        /**
         * Clamps all color components within the range of 0 to 1.
         */
        void Saturate();

        /**
         * Blends this color with another color respecting the alpha channel.
         * 
         * This method changes this color and leaves the other color
         * untouched, which is the most common usage of this method.
         * 
         * @param o the other color to blend with this color
         * @return a reference to this color used for method chaining etc.
         */
        Color & Blend(const Color & o);

        /**
         * Binary addition operator for two colors.
         *
         * @param rhs the right-hand side color
         * @return a new color representing the result of the operation
         */
        Color operator+(const Color & rhs) const;

        /**
         * Compound assignment and addition operator for two colors.
         *
         * @param rhs the right-hand side color
         * @return a reference to this color
         */
        Color& operator+=(const Color & o);

        /**
         * Binary subtraction operator for two colors.
         *
         * @param rhs the right-hand side color
         * @return a new color representing the result of the operation
         */
        Color operator-(const Color & o) const;

        /**
         * Compound assignment and subtraction operator for two colors.
         *
         * @param rhs the right-hand side color
         * @return a reference to this color
         */
        Color& operator-=(const Color & o);

        /**
         * Binary multiplication operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a new color representing the result of the operation
         */
        Color operator*(double s) const;

        /**
         * Compound assignment and multiplication operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this color
         */
        Color & operator*=(double s);

        /**
         * Binary division operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a new color representing the result of the operation
         */
        Color operator/(double s) const;

        /**
         * Compound assignment and division operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this color
         */
        Color & operator/=(double s);

        /**
         * Binary equality operator comparing two colors.
         *
         * @param o	the right-hand side color
         * @return `true` if the right-hand side color is equal to this color
         */
		bool operator==(const Color& o) const;

        /**
         * Binary non-equality operator comparing two colors.
         * 
         * Note: following common practice, this operator simply relied on the
         * binary equality operator and reverses the result.
         *
         * @param o	the right-hand side color
         * @return `true` if the right-hand side color is not equal to this color
         */
        bool operator!=(const Color& o) const {
            return !(*this == o);
        }         

        /**
         * Binary less-than operator comparing two colors.
         * 
         * This operator is required to have colors as keys within
         * std::map containers and in order to be able to 'sort' colors.
         * 
         * @param o	the right-hand side color
         * @return `true` if this color evaluates as 'less than' compared to the right-hand side color
         */
        bool operator<(const Color & rhs) const;
    };

    /**
     * This enumeration defines values for color names according to 
     * the W3C specification.
     * 
     * @ingroup gfx_group
     */
    enum WebColors {
        /** Color constant for the color *White* (0xFFFFFF). */
        White = 0xFFFFFF,

        /** Color constant for the color *Silver* (0xC0C0C0). */
        Silver = 0xC0C0C0,

        /** Color constant for the color *Gray* (0x808080). */
        Gray = 0x808080,

        /** Color constant for the color *Black* (0x000000). */
        Black = 0x000000,

        /** Color constant for the color *Red* (0xFF0000). */
        Red = 0xFF0000,

        /** Color constant for the color *Maroon* (0x800000). */
        Maroon = 0x800000,

        /** Color constant for the color *Yellow* (0xFFFF00). */
        Yellow = 0xFFFF00,

        /** Color constant for the color *Olive* (0x808000). */
        Olive = 0x808000, 

        /** Color constant for the color *Lime* (0x00FF00). */
        Lime = 0x00FF00,

        /** Color constant for the color *Green* (0x008000). */
        Green = 0x008000,

        /** Color constant for the color *Aqua* (0x00FFFF). */
        Aqua = 0x00FFFF,

        /** Color constant for the color *Teal* (0x008080). */
        Teal = 0x008080,

        /** Color constant for the color *Blue* (0x0000FF). */
        Blue = 0x0000FF,

        /** Color constant for the color *Navy* (0x000080). */
        Navy = 0x000080,

        /** Color constant for the color *Fuchsia* (0xFF00FF). */
        Fuchsia = 0xFF00FF,

        /** Color constant for the color *Purple* (0x800080). */
        Purple = 0x800080
    };

    /**
     * Binary multiplication operator for a scalar value and a color.
     *
     * @param s the left-hand side scalar value
     * @param c the right-hand color
     * @return a new color representing the result of the operation
     */
	inline const Color operator*(double s, const Color & c) {
		return c * s;
	}

} // end of namespace

/**
 * Insertion operator for color classes.
 * 
 * @param os    the output stream
 * @param c     the color
 * @return  reference to the output stream
 */
std::ostream& operator<<(std::ostream& os, const astu::Color& c);
