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

    class Vector2 {
    public:
        /** The x-coordinate of this vector. */
        double x;

        /** The y-coordinate of this vector. */
        double y;

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
        static double Length(double vx, double vy) {
            return std::sqrt(LengthSquared(vx, vy));
        }

        /**
         * Returns the length of a two-dimensional vector.
         * 
         * @param vx    the x-coordinate of the vector
         * @param vy    the y-coordinate of the vector
         * @return the length of the vector squared
         */
        static double LengthSquared(double vx, double vy) {
            return vx * vx + vy * vy;
        }

        /**
         * Constructor.
         * 
         * @param x the x-coordinate of the vector
         * @param y the y-coordinate of the vector
         */
        Vector2(double x = 0, double y = 0)
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
        Vector2 & Set(double x, double y) {
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
        Vector2 & SetX(double x) {
            this->x = x;
            return *this;
        }

        /**
         * Sets the y component of this vector.
         *
         * @param y the y-coordinate of the vector
         * @return reference to this vector for method chaining
         */
        Vector2 & SetY(double y) {
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
        double Length() const {
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
        Vector2 & SetLength(double l) {
            return *this *= l / Length();
        }

        /**
         * Returns the length of this vector squared.
         *
         * @return the length of this vector squared
         */
        double LengthSquared() const {
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
            double len = Length();
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
        double Distance(const Vector2 & o) const {
            return Length(x - o.x, y - o.y);
        }        

        /**
         * Returns the squared distance between this vector and the other vector.
         *
         * @param o the other vector
         * @return the distance squared
         */
        double DistanceSquared(const Vector2 & o) const {
            return LengthSquared(x - o.x, y - o.y);
        }

        Vector2 & Rotate(double phi)
        {
            double cosa = cos(phi);
            double sina = sin(phi);

            double xt = x * cosa - y * sina;
            y = y * cosa + x * sina;
            x = xt;

            return *this;
        }        

        double Dot(const Vector2 & o) const
        {
            return x * o.x + y * o.y;
        }

        double Dot(double vx, double vy) const
        {
            return x * vx + y * vy;
        }

        double Cross(double vx, double vy) const
        {
            return x * vy - y * vx;
        }

		double Cross(const Vector2& o) const
		{
			return x * o.y - y * o.x;
		}

        double Angle(const Vector2& ref) const
        {
            return std::atan2(Cross(ref), Dot(ref));
        }

        const Vector2 operator+(const Vector2 & right) const
        {
            return Vector2(x + right.x, y + right.y);
        }        

        Vector2 & operator+=(const Vector2 & right)
        {
            x += right.x;
            y += right.y;
            return *this;
        }

        const Vector2 operator-(const Vector2 & right) const
        {
            return Vector2(x - right.x, y - right.y);
        }

        Vector2 & operator-=(const Vector2 & right)
        {
            x -= right.x;
            y -= right.y;
            return *this;
        }

		const Vector2 operator*(const Vector2 & right) const
		{
    		return Vector2(x * right.x, y * right.y);
		}       

		Vector2 & operator*=(const Vector2 & right)
		{
			x *= right.x;
			y *= right.y;
			return *this;
		}   

        Vector2 & operator/=(double s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        Vector2 operator/(double s) const
        {
            return Vector2(x / s, y / s);
        }

        Vector2 operator*(double s) const
        {
            return Vector2(x * s, y * s);
        }

        Vector2 & operator*=(double s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        const Vector2 operator-() const {
            return Vector2(-x, -y);
        }        

		bool operator==(const Vector2& o) const {
			return x == o.x && y == o.y;
		}       

        bool operator!=(const Vector2& o) const {
            return !(*this == o);
        }         
    };

	inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
		os << '{' << vec.x << ", " << vec.y << '}';
		return os;
	}    

	inline const Vector2 operator*(double s, const Vector2 & v) {
		return v * s;
	}    
}