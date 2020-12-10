/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */
 
#include <cmath>
#include <iostream>

#pragma once

namespace astu {

    /**
	 * A two dimensional vector. 
     * 
     * @ingroup math_group
	 */
    template <typename T>
    class Vector2 {
    public:
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
         * Sets the x and y components of this vector.
         *
         * @param x the x-coordinate of the vector
         * @param y the y-coordinate of the vector
         * @return reference to this vector for method chaining
         */
        Vector2 & Set(T x, T y) {
            this->x = x;
            this->y = y;
            return *this;
        }

        /**
         * Sets the x component of this vector.
         *
         * @param x the x-coordinate of the vector
         * @return reference to this vector for method chaining
         */
        Vector2 & SetX(T x) {
            this->x = x;
            return *this;
        }

        /**
         * Sets the y component of this vector.
         *
         * @param y the y-coordinate of the vector
         * @return reference to this vector for method chaining
         */
        Vector2 & SetY(T y) {
            this->y = y;
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
}