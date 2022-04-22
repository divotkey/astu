/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard includes
#include <iostream>
#include <cmath>

namespace astu {
    
    /**
     * Represents a color value in RGB color space.
     * 
     * A color is described using four color channels: red, green, blue and
     * alpha. The alpha channel represents transparency. The color channels
     * are represents as floating-point values which normally lie within the
     * interval [0, 1].
     * 
     * @ingroup gfx_group
     */
    template <typename T>
    class Color final {
    public:
        /** The red color component. */
        T r;

        /** The green color component. */
        T g;

        /** The blue color component. */
        T b;

        /** The alpha color component. */
        T a;

        /**
         * Creates a color from RGB integer values.
         * 
         * @param red   red color component, value within the range 0 and 255
         * @param green green color component, value within the range 0 and 255
         * @param blue  blue color component, value within the range 0 and 255
         * @param alpha transparency, value within the range 0 and 255
         */
        static Color<T> CreateFromRgb(
            int red, int green, int blue, int alpha = 255)
        {
            return Color(
                red / static_cast<T>(255), 
                green / static_cast<T>(255), 
                blue / static_cast<T>(255), 
                alpha / static_cast<T>(255)
                );
        }

        /**
         * Creates a color from RGB integer values.
         *
         * @param rgb   the rgb color components encoded within in an integer
         */
        static Color<T> CreateFromRgba(int rgba)
        {
            return CreateFromRgb(
                    (rgba & 0xff000000) >> 24,
                    (rgba & 0xff0000) >> 16,
                    (rgba & 0xff00) >> 8,
                    rgba & 0xff);
        }

        /**
         * Constructor.
         * 
         * @param red   the red color component
         * @param green the green color component
         * @param blue  the blue color component
         * @param alpha the alpha color component
         */
        Color(T red = 0, T green = 0, T blue = 0, T alpha = 1)
            : r(red)
            , g(green)
            , b(blue)
            , a(alpha)
        {
            // Intentionally left empty.
        }

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
        Color(int rgb) {
            *this = CreateFromRgb(
                        (rgb & 0xff0000) >> 16, 
                        (rgb & 0xff00) >> 8, 
                        rgb & 0xff);
        }
        
        /**
         * Sets the alpha channel of this color.
         * 
         * @param a the alpha channel
         * @return a reference to this color for method chaining
         */
        Color<T> & SetAlpha(T a) {
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
        void Set(T red, T green, T blue, T alpha = 1) {
            r = red;
            g = green;
            b = blue;
            a = alpha;
        }

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
        T DistanceWithoutAlpha(const Color<T> & o) const
        {
            return std::sqrt( DistanceSquaredWithoutAlpha(o) );
        }

        /**
         * Calculates the Euclidean squared in RGB color space.
         * 
         * @param o the other color
         * @return the distance to the other color
         */
        T DistanceSquaredWithoutAlpha(const Color<T> & o) const {
            return (r - o.r) * (r - o.r) 
                 + (g - o.g) * (g - o.g)
                 + (b - o.b) * (b - o.b);
        }

        /**
         * Calculates the Euclidean in RGBA color space.
         * 
         * @param o the other color
         * @return the distance to the other color
         */
        T Distance(const Color<T> & o) const
        {
            return std::sqrt( DistanceSquared(o) );        
        }

        /**
         * Calculates the Euclidean squared in RGBA color space.
         * 
         * @param o the other color
         * @return the distance to the other color
         */
        T DistanceSquared(const Color<T> & o) const {
            return (r - o.r) * (r - o.r) 
                   + (g - o.g) * (g - o.g) 
                   + (b - o.b) * (b - o.b) 
                   + (a - o.a) * (a - o.a);
        }

        /**
         * Multiplies all color except alpha channel with a scalar.
         * 
         * @param s the scalar value
         * @return a reference to this color used for method chaining
         */
        Color<T> & MultiplyWithoutAlpha(T s) noexcept {
            r *= s;
            g *= s;
            b *= s;
            return *this;
        }

        /**
         * Converts this color to an integer value.
         * 
         * @return the integer representation of this color
         */
        int GetARGB() const {
            return  ((int)(a * 255) << 24)
                    | ((int)(r * 255) << 16)
                    | ((int)(g * 255) << 8)
                    | ((int)(b * 255));
        }


        /**
         * Converts this color to an integer value.
         * 
         * @return the integer representation of this color
         */
        int GetABGR() const {
            return  ((int)(a * 255) << 24)
                    | ((int)(b * 255) << 16)
                    | ((int)(g * 255) << 8)
                    | ((int)(r * 255));
        }

        /**
         * Clamps all color components within the range of 0 to 1.
         * 
         * @return a reference to this color used for method chaining
         */
        Color<T> & Saturate() noexcept {
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

            return *this;
        }

        /**
         * Converts this color from linear RGB color space to sRGB color space.
         * Values less than zero or greater than one are clamped to [0, 1].
         *
         * @return reference to this color for method chaining
         */
        Color<T> &ToSRGB() {
            r = ToSRGB(r);
            g = ToSRGB(g);
            b = ToSRGB(b);

            return *this;
        }

        /**
         * Blends this color with another color respecting the alpha channel.
         * 
         * This method changes this color and leaves the other color
         * untouched, which is the most common usage of this method.
         * 
         * @param o the other color to blend with this color
         * @return a reference to this color used for method chaining
         */
        Color<T> & Blend(const Color<T> & o) {
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

        /**
         * Does a linear interpolation between this and the specified color.
         * 
         * @param o the other color
         * @param t the interpolation position in the interval [0, 1]
         * @return the new interpolated color
         */
        Color<T> Lerp(const Color<T> & o, T t) const {
            return *this + (o - *this) * t;
        }

        /**
         * Tests whether all components are zero, ignoring the alpha component.
         *
         * @return `true` if all components are zero.
         */
        bool IsZeroWithoutAlpha() const {
            return r == 0 && b == 0 && g == 0;
        }

        /**
         * Tests whether all components are zero.
         *
         * @return `true` if all components are zero.
         */
        bool IsZero() const {
            return r == 0 && b == 0 && g == 0 && a == 0;
        }

        /**
         * Returns the maximum channel value of this color.
         *
         * @return the maximum value, either the red, green, blue or alpha channel
         */
        T GetMax() const {
            return std::max(r, std::max(g, std::max(b, a)));
        }

        /**
         * Clamps all channel values to the specified maximum.
         *
         * @param value the maximum channel value
         * @return reference to this color for method chaining
         */
        Color<T> &ClampMax(T value) {
            if (r > value) r = value;
            if (g > value) g = value;
            if (b > value) b = value;
            if (a > value) a = value;
            return *this;
        }

        /**
         * Clamps all channel values to the specified minimum.
         *
         * @param value the minimum channel value
         * @return reference to this color for method chaining
         */
        Color<T> &ClampMin(T value) {
            if (r < value) r = value;
            if (g < value) g = value;
            if (b < value) b = value;
            if (a < value) a = value;
            return *this;
        }

        /**
         * Binary addition operator for two colors.
         *
         * @param rhs the right-hand side color
         * @return a new color representing the result of the operation
         */
        Color<T> operator+(const Color<T> & rhs) const
        {
            return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
        }

        /**
         * Compound assignment and addition operator for two colors.
         *
         * @param rhs   the right-hand side color
         * @return a reference to this color
         */
        Color<T>& operator+=(const Color<T> & rhs) {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            a += rhs.a;
            return *this;
        }

        /**
         * Binary subtraction operator for two colors.
         *
         * @param rhs   the right-hand side color
         * @return a new color representing the result of the operation
         */
        Color<T> operator-(const Color<T> & rhs) const {
            return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
        }

        /**
         * Compound assignment and subtraction operator for two colors.
         *
         * @param rhs   the right-hand side color
         * @return a reference to this color
         */
        Color<T>& operator-=(const Color<T> & rhs) {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            a -= rhs.a;
            return *this;
        }

        /**
         * Binary multiplication operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a new color representing the result of the operation
         */
        Color<T> operator*(T s) const {
            return Color(r * s, g * s, b * s, a * s);
        }

        /**
         * Binary division operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a new color representing the result of the operation
         */
        Color<T> operator/(T s) const {
            return Color(r / s, g / s, b / s, a / s);
        }

        /**
         * Compound assignment and multiplication operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this color
         */
        Color<T> & operator*=(T s) {
            r *= s;
            g *= s;
            b *= s;
            a *= s;
            return *this;
        }

        /**
         * Binary multiplication operator for two colors.
         *
         * @param rhs the right-hand color value
         * @return a new color representing the result of the operation
         */
        Color<T> operator*(const Color<T> &rhs) const {
            return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
        }

        /**
         * Compound assignment and multiplication operator for two colors.
         *
         * @param rhs the right-hand color value
         * @return a reference to this color
         */
        Color<T> & operator*=(const Color<T> &rhs) {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            a *= rhs.a;
            return *this;
        }

        /**
         * Compound assignment and division operator for a color and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this color
         */
        Color<T> & operator/=(T s) {
            r /= s;
            g /= s;
            b /= s;
            a /= s;
            return *this;
        }

        /**
         * Binary division operator for two colors.
         *
         * @param rhs the right-hand color value
         * @return a new color representing the result of the operation
         */
        Color<T> operator/(const Color<T> &rhs) const {
            return Color(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
        }

        /**
         * Compound assignment and division operator for two colors.
         *
         * @param rhs the right-hand color value
         * @return a reference to this color
         */
        Color<T> & operator/=(const Color<T> &rhs) {
            r /= rhs.r;
            g /= rhs.g;
            b /= rhs.b;
            a /= rhs.a;
            return *this;
        }

        /**
         * Binary equality operator comparing two colors.
         *
         * @param o	the right-hand side color
         * @return `true` if the right-hand side color is equal to this color
         */
		bool operator==(const Color<T>& o) const {
            return r == o.r && g == o.g && b == o.b && a == o.a;
        }

        /**
         * Binary non-equality operator comparing two colors.
         * 
         * Note: following common practice, this operator simply relied on the
         * binary equality operator and reverses the result.
         *
         * @param o	the right-hand side color
         * @return `true` if the right-hand side color is not equal to this color
         */
        bool operator!=(const Color<T>& o) const {
            return !(*this == o);
        }         

        /**
         * Binary less-than operator comparing two colors.
         * 
         * This operator is required to have colors as keys within
         * std::map containers and in order to be able to 'sort' colors.
         * 
         * @param rhs   the right-hand side color
         * @return `true` if this color evaluates as 'less than' compared to the right-hand side color
         */
        bool operator<(const Color<T> & rhs) const {
            //XXX this implementation is slow, improvement required.
            double lng1 = sqrt(r * r + g * g + b * b + a * a);
            double lng2 = sqrt(rhs.r * rhs.r + rhs.g * rhs.g + rhs.b * rhs.b + rhs.a * rhs.a);
            return lng1 < lng2;
        }

    private:

        /**
         * Converts a single color channel from RGB color space to sRGB color space.
         *
         * Values less than zero or greater than one are clamped to [0, 1].
         *
         * @param x the color channel in linear RGB color space
         * @return the converted color channel in SRGB color space
         */
        T ToSRGB(T x) const {
            if (x <= static_cast<T>(0)) {
                return static_cast<T>(0);
            } else if (x >= static_cast<T>(1)) {
                return static_cast<T>(1);
            } else if (x < static_cast<T>(0.0031308)) {
                return x * static_cast<T>(12.92);
            } else {
                return std::pow(x, static_cast<T>(1.0 / 2.4)) * static_cast<T>(1.055) - static_cast<T>(0.055);
            }
        }

    };

    /**
     * Binary multiplication operator for a scalar value and a color.
     *
     * @param s the left-hand side scalar value
     * @param c the right-hand color
     * @return a new color representing the result of the operation
     */
    template <typename T>
	inline const Color<T> operator*(T s, const Color<T> & c) {
		return c * s;
	}

    /**
     * Binary division operator for a scalar value and a color.
     *
     * @param s the left-hand side scalar value
     * @param c the right-hand color
     * @return a new color representing the result of the operation
     */
    template <typename T>
    inline const Color<T> operator/(T s, const Color<T> & c) {
        return Color(s / c.r, s / c.g, s / c.b, s / c.a);
    }

    using Color4f = Color<float>;
    using Color4d = Color<double>;

} // end of namespace

/**
 * Insertion operator for color classes.
 * 
 * @param os    the output stream
 * @param c     the color
 * @return  reference to the output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const astu::Color<T>& c) {
    os << '{' << c.r << ", " << c.g << ", " << c.b << ", " << c.a << '}';
    return os;
}
