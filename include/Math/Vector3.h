/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <cmath>
#include <iostream>

namespace astu {

    /**
	 * A three dimensional vector. 
     * 
     * @ingroup math_group
	 */
    template <typename T>
	class Vector3 {
    public:
        // For some reasons, MS C++ compiler does not work with template inline keyword.
		///** Zero vector to be used as convenient constant. */
		//static const inline Vector3<T> Zero = Vector3<T>(
        //    static_cast<T>(0),
        //    static_cast<T>(0),
        //    static_cast<T>(0)
        //);

        /** Zero vector to be used as convenient constant. */
        static const Vector3<T> Zero;

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
         * Constructor that initializes all components with 0.
         */
        Vector3()
            : x(0), y(0), z(0)
        {
            // Intentionally left empty.
        }

        /**
         * Constructor.
         * 
         * @param vx	the x-coordinate
         * @param vy	thy y-coordinate
         * @param vz	thy z-coordinate
         */
        Vector3(T vx, T vy, T vz)
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
         * Sets this vector to the coordinates of another vector.
         *
         * @param o the other vector which coordinates to use
         * @return reference to this vector for method chaining
         */
        Vector3 & Set(const Vector3<T> & o) {
            x = o.x;
            y = o.y;
            z = o.z;
            return *this;
        }

        /**
         * Sets this vector to zero length.
         *
         * @return reference to this vector for method chaining
         */
        Vector3 &SetZero() {
            x = 0; y = 0; z = 0;
            return *this;
        }

        /**
         * Tests whether this vector has zero length.
         *
         * @return `true` if this vector has zero length
         */
        bool IsZero() const {
            return x == 0 && y == 0 && z == 0;
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
         * Sets the length of this vector.
         *
         * In case the current length of this vector is zero, the result
         * will be undefined.
         *
         * @param lng the new length
         * @return reference to this vector for method chaining
         */
        Vector3<T> &SetLength(T lng) {
            return *this *= lng / Length();
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
         * Flips this vector in-place.
         *
         * @return reference to this vector for method chaining
         */
        Vector3<T> & Flip() {
            x = -x;
            y = -y;
            z = -z;
            return *this;
        }

        /**
         * Calculates the dot product between this vector and the given vector.
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
        Vector3<T> & Cross(T vx, T vy, T vz) {
            Set(y * vz - z * vy, z * vx - x * vz, x * vy - y * vx);
            return *this;
        }

        /**
         * Returns the reflection vector of this incidence vector and a given normal vector.
         *
         * The normal vector n should be normalized. If nv is normalized, the output
         * vector will have the same length as this vector.
         *
         * @param nv    the normal vector
         * @return the reflection vector
         * @tparam T the numerical type of the vectors
         */
        Vector3<T> Reflect(const Vector3<T> &nv) const {
            return *this - nv * (static_cast<T>(2) * Dot(nv));
        }

        /**
         * Rotates this vector about the x axis.
         *
         * @param phi   the phi in radians
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateX(T phi) {
            T cosA = std::cos(phi);
            T sinA = std::sin(phi);
            T t = y * cosA - z * sinA;
            z = y * sinA + z * cosA;
            y = t;
            return *this;
        }

        /**
         * Rotates this vector about the x axis.
         *
         * @param phi   the angle in degrees
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateDegX(T phi) {
            return RotateX(MathUtils::ToRadians(phi));
        }

        /**
         * Rotates this vector about the y axis.
         *
         * @param phi   the phi in radians
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateY(T phi) {
            T cosA = std::cos(phi);
            T sinA = std::sin(phi);
            T t = z * sinA + x * cosA;
            z = z * cosA - x * sinA;
            x = t;
            return *this;
        }

        /**
         * Rotates this vector about the y axis.
         *
         * @param phi   the angle in degrees
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateDegY(T phi) {
            return RotateY(MathUtils::ToRadians(phi));
        }

        /**
         * Rotates this vector about the z axis.
         *
         * @param phi   the phi in radians
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateZ(T phi) {
            T cosA = std::cos(phi);
            T sinA = std::sin(phi);
            T t = x * cosA - y * sinA;
            y = x * sinA + y * cosA;
            x = t;
            return *this;
        }

        /**
         * Rotates this vector about the z axis.
         *
         * @param phi   the angle in degrees
         * @return reference to this vector for method chaining
         */
        Vector3<T> & RotateDegZ(T phi) {
            return RotateZ(MathUtils::ToRadians(phi));
        }

        /**
         * Compares this vector with another vector for equality.
         *
         * @param o the other vector
         * @param e the error range (epsilon)
         * @return `true` if the other vector is equal to this vector within a certain error range
         */
        bool IsEqual(const Vector3<T> o, T e = static_cast<T>(MathUtils::Epsilon)) const {
            return MathUtils::IsEqual(x, o.x, e) && MathUtils::IsEqual(y, o.y, e) && MathUtils::IsEqual(z, o.z, e);
        }

        /**
         * Verifies that this vector has length of approximately one unit.
         *
         * @return `true` if ths is a unit vector
         */
        bool IsUnitVector() const {
            return MathUtils::IsEqual(Length(), static_cast<T>(1));
        }

        /**
         * Accesses the element with the specified index.
         *
         * @param idx   index of the element to be retrieved
         * @return the requested element
         * @throws std::out_of_range in case the specified index is invalid
         */
        T& at(size_t idx) {
            if (idx < 3) {
                return *(this)[idx];
            }
            throw std::out_of_range("Index out of range");
        }

        /**
         * Accesses the element with the specified index.
         *
         * @param idx   index of the element to be retrieved
         * @return the requested element
         * @throws std::out_of_range in case the specified index is invalid
         */
        const T& at(size_t idx) const {
            if (idx < 3) {
                return *(this)[idx];
            }
            throw std::out_of_range("Index out of range");
        }

        /**
         * Subscript operator for this vector.
         *
         * @param idx   index of the element to be retrieved
         * @return the requested element
         */
        T& operator[](size_t idx) {
            return &x[idx];
        }

        /**
         * Subscript operator for this vector.
         *
         * @param idx   index of the element to be retrieved
         * @return the requested element
         */
        const T& operator[](size_t idx) const {
            return &x[idx];
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
         * @param rhs   the right hand-side vector
         * @return a new vector representing the result of the operation
         */
        const Vector3<T> operator-(const Vector3<T> & rhs) const {
            return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        /**
         * Compound assignment and subtraction operator for two vectors.
         *
         * @param rhs   the right hand-side vector
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
            return x == rhs.x && y == rhs.y && z == rhs.z;
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

    /**
     * Calculates the cross product between two vectors.
     * @param v1    the first vector
     * @param v2    the second vector
     * @return the cross product between the two vectors
     */
    template<typename T>
    Vector3<T> Cross(const Vector3<T> &v1, const Vector3<T> &v2) {
        return Vector3<T>(v1).Cross(v2);
    }

    /**
     * Returns a normalized version of a vector
     * @param v the vector to normalize
     * @return the normalized input vector
     */
    template<typename T>
    Vector3<T> Normalize(const Vector3<T> &v) {
        return Vector3<T>(v).Normalize();
    }

    template<typename T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T> &vec) {
		os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ']';
		return os;
	}

    /**
     * Defines the Zero-Constant für Vector2 templates.
     * (For some reasons, MS C++ compiler does not work with template inline
     * keyword.)
     *
     * @param tparam    the numerical type of the vector
     */
    template <typename T>
    Vector3<T> const Vector3<T>::Zero = Vector3<T>(0, 0, 0);

    /**
     * Convenient type alias for astu::Vector3 template using float as data type.
     */
    using Vector3f = Vector3<float>;

    /**
     * Convenient type alias for astu::Vector3 template using double as data type.
     */
    using Vector3d = Vector3<double>;

} // end of namespace