/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <cmath>
#include <iostream>


namespace astu {

    /**
	 * A three dimensional vector. 
	 */
    template <typename T>
	class Vector3 {
    public:
        /** The x-coordinate of this vector. */
        T x;

        /** The y-coordinate of this vector. */
        T y;

        /** The z-coordinate of this vector. */
        T z;

        /**
         * Returns the length of a vector.
         * 
         * This method uses a square root function and is computationally
         * expensive. If possible try to use `lengthSquared` instead.
         * 
         * @param vx	the x-coordinate of the vector
         * @param vy	the y-coordinate of the vector
         * @param vz	the z-coordinate of the vector
         * @return the length of the vector
         */
        static T Length(T vx, T vy, T vz) {
            return std::sqrt(vx * vx + vy * vy + vz * vz);
        }

        /**
         * Returns the length of the specified vector squared.
         * 
         * @param vx	the x-coordinate of the vector
         * @param vy	the y-coordinate of the vector
         * @param vz	the z-coordinate of the vector
         * @return the length of the vector
         */
        static T LengthSquared(T vx, T vy, T vz) {
            return vx * vx + vy * vy + vz * vz;
        }        

        /**
         * Constructor.
         * 
         * @param vx	the x-coordinate
         * @param vy	thy y-coordinate
         * @param vz	thy z-coordinate
         */
        Vector3(T vx = 0, T vy = 0, T vz = 0)
            : x(vx), y(vy), z(vz)
        {
            // intentionally left empty
        }

        /**
         * Sets this vector to the specified coordinates.
         *
         * @param vx    the new x-coordinate
         * @param vy	the new y-coordinate
         * @param vz	the new z-coordinate
         * @return reference to this vector for method chaining
         */
        Vector3 & Set(T vx, T vy, T vz) { 
            x = vx; y = vy; z = vz; 
            return *this; 
        }

        /**
         * Returns the length of this vector squared.
         *
         * @return the squared length of this vector
         */
        T LengthSquared() const {
            return x * x + y * y + z * z;
        }

        /**
         * Returns the length of this vector.
         * 
         * This method uses a square root function and is computationally
         * expensive. If possible use `lengthSquared` instead.
         *
         * @return the length of this vector
         */
        T Length() const {
            return std::sqrt(x * x + y * y + z * z);
        }  

        /**
         * Returns the distance between this vector and the other vector.
         * The vectors are assumed to be points in 3 dimensional space. 
         *
         * This method uses a square root function and is computationally
         * expensive. If possible try to use `distanceSquared` instead.
         *
         * @param o the other vector
         * @return the distance between the two points
         */
        T Distance(const Vector3<T> & o) const {
            return Length(x - o.x, y - o.y, z - o.z);
        }

        /**
         * Returns the distance between this vector and the other vector.
         * The vectors are assumed to be points in 3 dimensional space. 
         *
         * This method uses a square root function and is computationally
         * expensive. If possible try to use `distanceSquared` instead.
         *
         * @param vx	the x coordinate of the other vector
         * @param vy	the y coordinate of the other vector
         * @param vz	the z coordinate of the other vector
         * @return the distance between the two points
         */
        T Distance(T vx, T vy, T vz) const {
            return Length(x - vx, y - vy, z - vz);
        }           

        /**
         * Returns the distance between this vector and the other vector squared.
         * The vectors are assumed to be points in 3 dimensional space. 
         *
         * @param o the other vector
         * @return the distance between the two points
         */
        T DistanceSquared(const Vector3<T> & o) const {
            return LengthSquared(x - o.x, y - o.y, z - o.z);
        }

        /**
         * Returns the distance between this vector and the other vector squared.
         * The vectors are assumed to be points in 2 dimensional space. 
         *
         * @param vx	the x coordinate of the other vector
         * @param vy	the y coordinate of the other vector
         * @param vz	the z coordinate of the other vector
         * @return the distance between the two points
         */
        T DistanceSquared(T vx, T vy, T vz) const {
            return LengthSquared(x - vx, y - vy, z - vz);
        }

        /**
         * Normalizes this vector.
         * This method does not test if this vector has zero length. 
         * 
         * @return reference to this vector for method chaining
         */
        Vector3<T> & Normalize() {
            T lng = Length();
            x /= lng;
            y /= lng;
            z /= lng;
            return *this;
        }

        /**
         * Normalizes this vector.
         * If this vector has zero length, this method has no effect.
         *
         * @return reference to this vector for method chaining
         */
        Vector3<T> & NormalizeSafe() {
            T lng = Length();
            if (lng != 0) {
                x /= lng;
                y /= lng;
                z /= lng;
            }
            return *this;
        }

        /**
         * Calcualtes the dot product between this vector and the given vector.
         *
         * @param o the other vector
         * @return the dot product between the two vectors.
         */
        T Dot(const Vector3<T> & o) const {
            return x * o.x + y * o.y + z * o.z;
        } 

        /**
         * Calculates the dot product between this vector and the given vector.
         *
         * @param vx    x coordinate of the other vector
         * @param vy    y coordinate of the other vector
         * @param vz	z coordinate of the other vector
         * @return the dot product between the two vectors.
         */
        T Dot(T vx, T vy, T vz) const {
            return x * vx + y * vy + z * vz;
        }               

        /**
         * Sets this vector to the cross product of this vector and the given vector.
         *
         * @param o the other vector
         * @return reference to this vector for method chaining
         */
        Vector3<T> & Cross(const Vector3<T> & o) {
            Set(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
            return *this;
        } 

        /**
         * Sets this vector to the cross product of this vector and the given vector.
         *
         * @param vx	x coordinate of the other vector
         * @param vy	y coordinate of the other vector
         * @param vz	z coordinate of the other vector
         * @return reference to this vector for method chaining
         */
        Vector3<T> & cross(T vx, T vy, T vz) {
            Set(y * vz - z * vy, z * vx - x * vz, x * vy - y * vx);
            return *this;
        } 

        /**
         * Rotates this vector about the x axis.
         *
         * @param angle	the angle in radians
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateX(T angle) {
            T cosA = std::cos(angle);
            T sinA = std::sin(angle);
            T t = y * cosA - z * sinA;
            z = y * sinA + z * cosA;
            y = t;
            return *this;
        }

        /**
         * Rotates this vector about the y axis.
         *
         * @param angle		the angle in radians
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateY(T angle) {
            T cosA = std::cos(angle);
            T sinA = std::sin(angle);
            T t = z * sinA + x * cosA;
            z = z * cosA - x * sinA;
            x = t;
            return *this;
        } 

        /**
         * Rotates this vector about the z axis.
         *
         * @param angle		the angle in radians
         * @return reference to this vector for method chaining
         */
        Vector3<T> & rotateZ(T angle) {
            T cosA = std::cos(angle);
            T sinA = std::sin(angle);
            T t = x * cosA - y * sinA;
            y = x * sinA + y * cosA;
            x = t;
            return *this;
        }

        /**
         * Binary addition operator for two vectors.
         *
         * @param rhs the right hand-side vector
         * @return a new vector representing the result of the operation
         */
        const Vector3<T> operator+(const Vector3<T> & rhs) const {
            return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        /**
         * Compound assignment and addition operator for two vectors.
         *
         * @param rhs the right hand-side vector
         * @return a reference to this vector
         */
        Vector3<T> & operator+=(const Vector3<T> & rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        /**
         * Binary subtraction operator for two vectors.
         *
         * @param right the right hand-side vector
         * @return a new vector representing the result of the operation
         */
        const Vector3<T> operator-(const Vector3<T> & rhs) const {
            return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        /**
         * Compound assignment and subtraction operator for two vectors.
         *
         * @param right the right hand-side vector
         * @return a reference to this vector
         */
        Vector3<T> & operator-=(const Vector3<T> & rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        /**
         * Binary multiplication operator for a vector and a scalar value
         *
         * @param s the right-hand side scalar value
         * @return a new vector representing the result of the operation
         */
        const Vector3<T> operator*(T s) const {
            return Vector3(x * s, y * s, z * s);
        }

        /**
         * Compound assignment and multiplication operator for a vector and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this vector
         */
        Vector3<T> & operator*=(T s) {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        /**
         * Binary division operator for a vector and a scalar value
         *
         * @param s the right-hand side scalar value
         * @return a new vector representing the result of the operation
         */
        const Vector3<T> operator/(T s) const {
            return Vector3(x / s, y / s, z / s);
        }

        /**
         * Compound assignment and division operator for a vector and a scalar value.
         *
         * @param s the right-hand side scalar value
         * @return a reference to this vector
         */
        Vector3<T> & operator/=(T s) {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        /**
         * Unary minus operation.
         * This operation changes the sign of all components of this vector. The resulting vector
         * will point into the opposite direction.
         *
         * @return a new vector representing the result of the operation
         */
        const Vector3<T> operator-() const {
            return Vector3(-x, -y, -z);
        }

        /**
         * Binary equality operator comparing two vectors.
         * 
         * @param rhs   the right hand side vector
         * @return `true` if the specified vector is equal to this vector
         */
        bool operator==(const Vector3<T> & rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.y;
        }

        /**
         * Binary non-equality operator comparing two vectors.
         *
         * @param rhs   the right hand side vector
         * @return `true` if the specified vector is not equal to this vector
         */
        bool operator!=(const Vector3& rhs) const {
            return !(*this == rhs);
        }
    };

	/**
	 * Binary multiplication operator for a scalar and a vector.
	 * This operator does a component-wise multiplication with the scalar value and the vector.
	 *
	 * @param s	the scalar value
	 * @param v	the vector
	 * @return a new vector representing the result of the operation
	 */
    template<typename T>
	const Vector3<T> operator*(T s, const Vector3<T>& v) {
		return v * s;
	}

    template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T> &vec) {
		os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ']';
		return os;
	}    

    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;

} // end of namespace