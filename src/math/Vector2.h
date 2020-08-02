/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */
 
#include <cmath>
#include <iostream>

#pragma once

namespace astu {

    class Vector2 {
    public:
        double x;
        double y;

        Vector2(double _x = 0, double _y = 0) : x(_x), y(_y) {}

        Vector2 & Set(double _x, double _y) {
            x = _x;
            y = _y;
            return *this;
        }

        Vector2 & SetZero() {
            x = 0; y = 0;
            return *this;
        }

        double LengthSquared() const {
            return x * x + y * y;
        }

        double Length() const {
            return std::sqrt(x * x + y * y);
        }

        Vector2 & Normalize() {
            double len = Length();
            x /= len;
            y /= len;
            return *this;			
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

        Vector2 operator/(double s)
        {
            return Vector2(x / s, y / s);
        }

        Vector2 operator*(double s)
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