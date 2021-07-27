/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard library includes
#include <cmath>
#include <iostream>

// Local includes
#include "MathUtils.h"

namespace astu {

    /**
	 * A two dimensional vector. 
     * 
     * @ingroup math_group
	 */
    template <typename T>
    class Vector2 {
    public:
		/** Zero vector to be used as convenient constant. */
		static const inline Vector2<T> Zero 
            = Vector2<T>(static_cast<T>(0), static_cast<T>(0));

        /** The x-coordinate of this vector. */
        T x;

        /** The y-coordinate of this vector. */
        T y;

        /**
         * Returns the length of a two dimensional vector.
         * 
         * This method uses a square root function and computationally
         * expensive. If possible try to use `LengthSquared` instead.
         * 
         * @param vx    the x-coordinate of the vector
         * @param vy    the y-coordinate of the vector
         * @return the length of the vector
         */
        static T Length(T vx, T vy) {
            return std::sqrt(LengthSquared(vx, vy));
        }

        /**
         * Returns the length of a two-dimensional vector.
         * 
         * @param vx    the x-coordinate of the vector
         * @param vy    the y-coordinate of the vector
         * @return the length of the vector squared
         */
        static T LengthSquared(T vx, T vy) {
            return vx * vx + vy * vy;
        }

        /**
         * Constructor.
         * 
         * @param x the x-coordinate of the vector
         * @param y the y-coordinate of the vector
         */
        Vector2(T x = 0, T y = 0)
            : x(x), y(y)
        {
            // Intentionally left empty.
        }

        /**
         * Sets thsi vector to the coordinates of another vector.
         *
         * @param o the other vector which coordinates to use
         * @return reference to this vector for method chaining
         */
        Vector2 & Set(const Vector2<T> & o) {
            this->x = o.x;
            this->y = o.y;
            return *this;
        }

        /**
         * Sets the x and y components of this vector.
         *
         * @param newX  the x-coordinate of the vector
         * @param newY  the y-coordinate of the vector
         * @return reference to this vector for method chaining
         */
        Vector2 & Set(T newX, T newY) {
			x = newX;
            y = newY;
            return *this;
        }
        
        /**
         * Sets the x component of this vector.
         *
         * @param _x    the x-coordinate of the vector
         * @return reference to this vector for method chaining
         */
        Vector2& SetX(T _x) {
            x = _x;
            return *this;
        }

        /**
         * Sets the y component of this vector.
         *
         * @param _y    the y-coordinate of the vector
         * @return reference to this vector for method chaining
         */
        Vector2& SetY(T _y) {
            y = _y;
            return *this;
        }

        /**
         * Sets this vector to zero length.
         * 
         * @return reference to this vector for method chaining
         */
        Vector2 & SetZero() {
            x = 0; y = 0;
            return *this;
        }

        /**
         * Tests whether this vector has zero length.
         * 
         * @return `true` if this vector has zero length
         */
        bool IsZero() const {
            return x == 0 && y == 0;
        }

        /**
         * Returns the length of this vector.#
         * 
         * This method uses a square root function and computationally
         * expensive. If possible try to use `LengthSquared` instead.
         *
         * @return the length of this vector
         */
        T Length() const {
            return std::sqrt(x * x + y * y);
        }

        /**
         * Sets the length of this vector to a certain length.
         * 
         * In case the current length of this vector is zero, the result
         * will be undefined.
         *
         * @param l	the new length of this vector
         * @return reference to this vector for method chaining
         */
        Vector2 & SetLength(T l) {
            return *this *= l / Length();
        }

        /**
         * Returns the length of this vector squared.
         *
         * @return the length of this vector squared
         */
        T LengthSquared() const {
            return x * x + y * y;
        }

        /**
         * Normalizes this vector.
         * 
         * This method does not test if this vector has zero length. 
         *
         * @return reference to this vector for method chaining
         */
        Vector2 & Normalize() {
            T len = Length();
            x /= len;
            y /= len;
            return *this;			
        }

        /**
         * Flips this vector in-place.
         * 
         * @return reference to this vector for method chaining
         */
        Vector2 & Flip() { 
            x = -x; 
            y = -y;  
            return *this;
        }

        /**
         * Returns the distance between this vector and the other vector.
         *
         * @param o the other vector
         * @return the distance between the two points
         */
        T Distance(const Vector2<T> & o) const {
            return Length(x - o.x, y - o.y);
        }        

        /**
         * Returns the squared distance between this vector and the other vector.
         *
         * @param o the other vector
         * @return the distance squared
         */
        T DistanceSquared(const Vector2<T> & o) const {
            return LengthSquared(x - o.x, y - o.y);
        }

        /**
         * Rotates this vector in-place.
         * 
         * @param phi   the angle in radians
         * @return reference to this vector for method chaining
         */
        Vector2 & Rotate(T phi)
        {
            T cosa = std::cos(phi);
            T sina = std::sin(phi);

            T xt = x * cosa - y * sina;
            y = y * cosa + x * sina;
            x = xt;

            return *this;
        }        

        /**
         * Rotates this vector in-place.
         * 
         * @param phi   the angle in degrees
         * @return reference to this vector for method chaining
         */
        Vector2 & RotateDeg(T phi)
        {
            return Rotate(ToRadians<T>(phi));
        }

        /**
         * Calculates the dot product.
         * 
         * @param o the the other vector
         * @return the dot product between the two vectors
         */
        T Dot(const Vector2<T> & o) const
        {
            return x * o.x + y * o.y;
        }

        /**
         * Calculates the dot product.
         * 
         * @param vx    the x-coordinate the the other vector
         * @param vy    the y-coordinate the the other vector
         * @return the dot product between the two vectors
         */
        T Dot(T vx, T vy) const
        {
            return x * vx + y * vy;
        }

        /**
         * Calculates the dot product.
         * 
         * @param vx    the x-coordinate the the other vector
         * @param vy    the y-coordinate the the other vector
         * @return the cross product of the two vectors
         */
        T Cross(T vx, T vy) const
        {
            return x * vy - y * vx;
        }

        /**
         * Calculates the dot product.
         * 
         * @param o the the other vector
         * @return the cross product of the two vectors
         */
		T Cross(const Vector2<T> &o) const
		{
			return x * o.y - y * o.x;
		}

        /**
         * Calculates the angle between this vector and a reference vector.
         * 
         * @param ref   the reference vector
         * @return the angle in radians
         */
        T Angle(const Vector2<T> &ref) const
        {
            return std::atan2(Cross(ref), Dot(ref));
        }

        /**
         * Sets this vector to perpendicular version of itself.
         * 
         * @return reference to this vector for method chaining
         */
        Vector2<T> & Perpendicularize()
        {
            T tmp = x;
            x = -y;
            y = tmp;
            return *this;
        }

        /**
         * Adds the specified vector to this vector in-place.
         *
         * @param vx	the x-coordinate of the other vector
         * @param vy	the y-coordinate of the other vector
         * @return reference to this vector for method chaining
         */
        Vector2<T> & Add(T vx, T vy)
        {
            x += vx; y += vy;
            return *this;
        }

        /**
         * Adds the specified vector to this vector in-place.
         *
         * @param o		the other vector
         * @return reference to this vector for method chaining
         */
        Vector2<T> & Add(const Vector2<T> & o)
        {
            x += o.x; y += o.y;
            return *this;
        }

        /**
         * Scales this vector by the specified scaling vector.
         * 
         * @param sv the scaling vector
         * @return reference to this vector for method chaining
         */
        Vector2<T> & Scale(const Vector2<T> & sv)
        {
            x *= sv.x;
            y *= sv.y;
            return *this;
        }       

        /**
         * Scales this vector by the specified vector.
         * 
         * @param sx the x-component of the scaling vector
         * @param sy the y-component of the scaling vector
         * @return reference to this vector for method chaining
         */
        Vector2<T> & Scale(T sx, T sy)
        {
            x *= sx;
            y *= sy;
            return *this;
        }


        /**
         * Binary addition operator for two vectors.
         *
         * @param right the right hand-side vector
         * @return a new vector representing the result of the operation
         */
        Vector2<T> operator+(const Vector2<T> &right) const
        {
            return Vector2(x + right.x, y + right.y);
        }        

        /**
         * Compound assignment and addition operator for two vectors.
         *
         * @param right     the right hand-side vector
         * @return a reference to this vector
         */
        Vector2<T> & operator+=(const Vector2<T> &right)
        {
            x += right.x;
            y += right.y;
            return *this;
        }

        /**
         * Binary subtraction operator for two vectors.
         *
         * @param right the right hand-side vector
         * @return a new vector representing the result of the operation
         */
        const Vector2<T> operator-(const Vector2<T> & right) const
        {
            return Vector2(x - right.x, y - right.y);
        }

        /**
         * Compound assignment and subtraction operator for two vectors.
         *
         * @param right     the right hand-side vector
         * @return a reference to this vector
         */
        Vector2<T> & operator-=(const Vector2<T> & right)
        {
            x -= right.x;
            y -= right.y;
            return *this;
        }

        /**
         * Binary multiplication operator for two vectors.
         * 
		 * This operator does a component-wise multiplication of the two vectors.
         *
         * @param right the right hand-side vector
         * @return a new vector representing the result of the operation
         */
		const Vector2<T> operator*(const Vector2<T> & right) const
		{
    		return Vector2(x * right.x, y * right.y);
		}       

        /**
         * Compound multiplication and subtraction operator for two vectors.
         * 
		 * This operator does a component-wise multiplication of the two vectors.
         *
         * @param right the right hand-side vector
         * @return a reference to this vector
         */
		Vector2<T> & operator*=(const Vector2<T> & right)
		{
			x *= right.x;
			y *= right.y;
			return *this;
		}   

        /**
         * Compound assignment and division operator with a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this vector
         */
        Vector2<T> & operator/=(T s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        /**
         * Binary division operator with a scalar value
         *
         * @param s the right-hand side scalar value
         * @return a new vector representing the result of the operation
         */
        Vector2<T> operator/(T s) const
        {
            return Vector2(x / s, y / s);
        }

        /**
         * Binary multiplication operator with a scalar value
         *
         * @param s the right-hand side scalar value
         * @return a new vector representing the result of the operation
         */
        Vector2<T> operator*(T s) const
        {
            return Vector2(x * s, y * s);
        }

        /**
         * Compound assignment and multiplication operator with a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this vector
         */
        Vector2<T> & operator*=(T s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        /**
         * Unary minus operation.
         * This operation changes the sign of all components of this vector.
         * The resulting vector will point into the opposite direction.
         *
         * @return a new vector representing the result of the operation
         */
        const Vector2<T> operator-() const {
            return Vector2(-x, -y);
        }        

        /**
         * Binary equality operator comparing two vectors.
         *
         * @param right the right hand side vector
         * @return `true` if the specified vector is equal to this vector
         */
		bool operator==(const Vector2<T> &right) const {
			return x == right.x && y == right.y;
		}       

        /**
         * Binary non-equality operator comparing two vectors.
         *
         * @param right the right hand side vector
         * @return `true` if the specified vector is not equal to this vector
         */
        bool operator!=(const Vector2<T> &right) const {
            return !(*this == right);
        }         
    };

    template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vector2<T> &vec) {
		os << '{' << vec.x << ", " << vec.y << '}';
		return os;
	}    

    template<typename T>
	inline const Vector2<T> operator*(T s, const Vector2<T> & v) {
		return v * s;
	}    

    /**
     * Convenient type alias for astu::Vector2 template using double as data type.
     */
    using Vector2d = astu::Vector2<double>;    

    /**
     * Convenient type alias for astu::Vector2 template using float as data type.
     */
    using Vector2f = astu::Vector2<float>;    

} // end of namespace