/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Vector3.h"
#include "MathUtils.h"

// C++ Standard Library includes
#include <cassert>
#include <cmath>
#include <algorithm>
#include <string>

namespace astu {

    /**
     * A quaternion class.
     *
     * Quaternions are commonly used to represent rotations in three-dimensional space.
     */
    template <typename T>
    class Quaternion {
    public:

        /** Identity quaternion constant. */
        const static Quaternion IDENTITY;

        /**
         * Creates a new instance. The new instance will be set to identity.
         */
        Quaternion() { SetToIdentity(); }

        /**
         * Creates a new quaternion with the specified components.
         *
         * @param qw	the w-component
         * @param qx	the x-component
         * @param qy	the y-component
         * @param qz	the z-component
         */
        Quaternion(T qw, T qx, T qy, T qz)
                : x(qx), y(qy), z(qz), w(qw)
        {
            // intentionally left empty
        }

        /**
         * Creates a new quaternion with the specified w-component and axis vector.
         *
         * @param qw	the w-component
         * @param axis	the axis vector
         */
        Quaternion(T qw, const Vector3<T> & v)
                : x(v.x), y(v.y), z(v.z), w(qw)
        {
            // intentionally left empty
        }

        /**
         * Sets this quaternion to be equal with the specified quaternion.
         *
         * @param o		the other quaternion
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Set(const Quaternion<T>& o) {
            x = o.x; y = o.y; z = o.z; w = o.w;
            return *this;
        }

        /**
         * Sets this quaternion to the specified component.
         *
         * @param qw	the w-component
         * @param qx	the x-component
         * @param qy	the y-component
         * @param qz	the z-component
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Set(T qw, T qx, T qy, T qz)
        {
            x = qx; y = qy;	z = qz;	w = qw;
            return *this;
        }

        /**
         * Sets this quaternion to the specified w-component and axis vector.
         *
         * @param qw	the w-component
         * @param axis	the axis vector
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Set(T qw, const Vector3<T> & axis)
        {
            x = axis.x;	y = axis.y;	z = axis.z; w = qw;
            return *this;
        }

        /**
         * Subscript operator for this quaternion.
         *
         * @param idx	index of the element to be retrieved
         * @return the requested element
         */
        T& operator[](size_t idx) {
            return _data[idx];
        }

        /**
         * Subscript operator for this quaternion.
         *
         * @param idx	index of the element to be retrieved
         * @return the requested element
         */
        const T& operator[](size_t idx) const {
            return _data[idx];
        }

        /**
         * Accesses the element with the specified index.
         *
         * @param idx	index of the element to be retrieved
         * @return the requested element
         * @throws std::out_of_range in case the specified index is invalid
         */
        T& at(size_t idx) {
            switch (idx) {
                case 0:
                    return x;

                case 1:
                    return y;

                case 2:
                    return z;

                case 3:
                    return w;

                default:
                    throw std::out_of_range("index out of range " + std::to_string(idx));
            }
        }

        /**
         * Accesses the element with the specified index.
         *
         * @param idx	index of the element to be retrieved
         * @return the requested element
         * @throws std::out_of_range in case the specified index is invalid
         */
        const T& at(size_t idx) const {
            switch (idx) {
                case 0:
                    return x;

                case 1:
                    return y;

                case 2:
                    return z;

                case 3:
                    return w;

                default:
                    throw std::out_of_range("index out of range " + std::to_string(idx));
            }
        }


        /**
         * Sets this quaternion to the identity quaternion.
         *
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& SetToIdentity()
        {
            return Set(1, 0, 0, 0);
        }

        /**
         * Calculates the length squared of this quaternion.
         *
         * @return the length^2
         */
        T LengthSquared() const {
            return  x * x + y * y + z * z + w * w;
        }

        /**
         * Calculates the length of this quaternion.
         *
         * @return the length
         */
        T Length() const {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        /**
         * Sets the length of this quaternion to one.
         * This method does not test if this quaternion has zero length.
         *
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Normalize() {
            T lng = Length();
            x /= lng;
            y /= lng;
            z /= lng;
            w /= lng;
            return *this;
        }

        /**
         * Sets the length of this quaternion to one.
         * If this quaternion has zero length, this method has no effect.
         *
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& NormalizeSafe() {
            T lng = Length();
            if (lng != 0) {
                x /= lng;
                y /= lng;
                z /= lng;
                w /= lng;
            }
            return *this;
        }

        /**
         * Sets the length of this quaternion to one. This version of
         * normalize avoids unnecessary calculations if the quaternion
         * is already of length one.
         *
         * @param epsilon	the margin of error used to test the (squared) length
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& NormalizeLazy(T epsilon = static_cast<T>(0.00000001)) {
            T lng2 = LengthSquared();
            if (lng2 != 0 && abs(lng2 - 1) > epsilon) {
                T lng = std::sqrt(lng2);
                x /= lng;
                y /= lng;
                z /= lng;
                w /= lng;
            }
            return *this;
        }

        /**
         * Conjugates this quaternion in-place.
         *
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Conjugate() {
            x = -x; y = -y; z = -z;
            return *this;
        }

        /**
         * Sets this quaternion to the given Euler angles.
         *
         * @param yaw		rotation about the y axis in radians
         * @param pitch		rotation about the x axis in radians
         * @param roll		rotation about the z axis in radians
         *
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& SetToRotation(T yaw, T pitch, T roll) {
            T hr = roll  * static_cast<T>(0.5),	shr = std::sin(hr),	chr = std::cos(hr);
            T hp = pitch * static_cast<T>(0.5),	shp = std::sin(hp),	chp = std::cos(hp);
            T hy = yaw   * static_cast<T>(0.5),	shy = std::sin(hy),	chy = std::cos(hy);

            T chy_shp = chy * shp;
            T shy_chp = shy * chp;
            T chy_chp = chy * chp;
            T shy_shp = shy * shp;

            x = (chy_shp * chr) + (shy_chp * shr);
            y = (shy_chp * chr) - (chy_shp * shr);
            z = (chy_chp * shr) - (shy_shp * chr);
            w = (chy_chp * chr) + (shy_shp * shr);

            return *this;
        }

        /**
         * Sets this quaternion to the given Euler angles. Calling this method with
         * a vector v is equivalent to calling: 'setToRotation(v.x, v.y, v.z)`.
         *
         * @param v		a vector holding the Euler angles in radians
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& SetToRotation(const Vector3<T>& angles) {
            return SetToRotation(angles.x, angles.y, angles.z);
        }

        /**
         * Extracts the Euler angles represented by this quaternion.
         *
         * @param a result vector where to store the Euler angles
         * @return a reference to the result vector
         */
        Vector3<T>& GetEulerAngles(Vector3<T>& result) const {
            GetEulerAngles(result.x, result.y, result.z);
            return result;
        }

        /**
         * Extracts the Euler angles represented by this quaternion.
         *
         * @return a vector holding the Euler angles
         */
        Vector3<T> GetEulerAngles() const {
            T yaw, pitch, roll;
            GetEulerAngles(yaw, pitch, roll);
            return Vector3<T>(yaw, pitch, roll);
        }

        /**
         * Extracts the Euler angles represented by this quaternion.
         *
         * @param yaw		stores the rotation about the y axis in radians
         * @param pitch		stores the rotation about the x axis in radians
         * @param roll		stores the rotation about the z axis in radians
         */
        void GetEulerAngles(T& yaw, T& pitch, T& roll) const {
            // yaw (y-axis rotation)
            yaw = std::atan2(2 * (y * w + x * z), 1 - 2 * (y * y + x * x));

            // pitch (x-axis rotation)
            pitch = std::asin(MathUtils::Clamp(2 * (w * x - z * y), static_cast<T>(-1), static_cast<T>(1)));

            //roll (z-axis rotation)
            roll = std::atan2(2 * (w * z + y * x), 1 - 2 * (x * x + z * z));
        }

        /**
         * Multiplies this quaternion with the specified quaternion in-place (this = this * o).
         *
         * @param o		the other quaternion
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Mul(const Quaternion<T> & o) {
            return *this *= o;
        }

        /**
         * Multiplies this quaternion with the specified quaternion in-place (this = o * this).
         *
         * @param o		the other quaternion
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& PreMul(const Quaternion<T> & o) {
            T x = o.w * this->x + o.x * this->w + o.y * this->z - o.z * this->y;
            T y = o.w * this->y + o.y * this->w + o.z * this->x - o.x * this->z;
            T z = o.w * this->z + o.z * this->w + o.x * this->y - o.y * this->x;
            T w = o.w * this->w - o.x * this->x - o.y * this->y - o.z * this->z;
            Set(w, x, y, z);

            return *this;
        }

        /**
         * Multiplies each component of this quaternion with the given scalar in-place.
         *
         * @param s		the scalar value
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Mul(T s) {
            x *= s; y *= s; z *= s; w *= s;
            return *this;
        }

        /**
         * Scales the components of the specified quaternion by the given scalar and
         * adds the result to this quaternion in-place.
         *
         * @param o		the other quaternion
         * @param s		the scaler
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& AddScaled(const Quaternion<T>& o, T s) {
            x += o.x * s; y += o.y * s; z += o.z * s; w += o.w * s;
            return *this;
        }

        /**
         * Adds the specified quaternion to this quaternion in-place.
         *
         * @param o		the other quaternion
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Add(const Quaternion<T>& o) {
            x += o.x; y += o.y; z += o.z; w += o.w;
            return *this;
        }

        /**
         * Subtracts the specified quaternion from this quaternion in-place.
         *
         * @param o		the other quaternion
         * @return reference to this quaternion for method chaining
         */
        Quaternion<T>& Sub(const Quaternion<T>& o) {
            x -= o.x; y -= o.y; z -= o.z; w -= o.w;
            return *this;
        }

        /**
         * Transforms (rotates) the specified vector by this quaternion. The result
         * is stored in the given vector.
         *
         * @param v		the vector to be transformed
         * @reference to the transformed input vector
         */
        Vector3<T>& Transform(Vector3<T>& v) const {
            Quaternion<T> tmp(*this);
            tmp.Conjugate().PreMul(Quaternion(0, v)).PreMul(*this);
            return v.Set(tmp.x, tmp.y, tmp.z);
        }

        /**
         * Transforms (rotates) the specified vector by this quaternion. The result
         * is stored in the given output vector. The input vector and the output
         * vector can be identical.
         *
         * @param v			the vector to be transformed
         * @param result	the result of the transformation
         * @result reference to the result vector
         */
        Vector3<T>& Transform(const Vector3<T>& v, Vector3<T> & result) const {
            Quaternion tmp(*this);
            tmp.Conjugate().PreMul(Quaternion(0, v)).PreMul(*this);
            return result.Set(tmp.x, tmp.y, tmp.z);
        }

        /**
         * Calculates the angular velocity based on the specified previous
         * orientation and delta time. The angular is represented by a
         * vector, where the directions represents the rotation axis and
         * the length the rotation speed in radians per second.
         *
         * Both quaternions must be normalized.
         *
         * @param q0	the previous orientation
         * @param dt	the delta time between the two orientations in seconds
         * @return vector describing the angular velocity
         */
        Vector3<T> AngularVelocity(const Quaternion<T> & q0, T dt) const {
            return AngularVelocity(q0, dt, Vector3<T>());
        }

        /**
         * Calculates the angular velocity based on the specified previous
         * orientation and delta time. The angular is represented by a
         * vector, where the directions represents the rotation axis and
         * the length the rotation speed in radians per second.
         *
         * Both quaternions must be normalized.
         *
         * @param q0	the previous orientation
         * @param dt	the delta time between the two orientations in seconds
         * @param result	the vector used to store the angular velocity
         * @return reference to the result vector
         */
        Vector3<T>& AngularVelocity(const Quaternion<T> & q0, T dt, Vector3<T> & result) const {
            assert(IsNormalized());
            assert(q0.isNormalized());

            Quaternion r(*this);
            r *= Quaternion(q0).conjugate();
            T theta = 2 * std::acos(std::max(static_cast<T>(-1), std::min(static_cast<T>(1), r.w)));
            if (theta > static_cast<T>(MathUtils::PId)) {
                theta -= static_cast<T>(MathUtils::PI2d);
            }

            if (theta != 0) {
                result.set(r.x, r.y, r.z).normalize().scale(theta / dt);
            }
            else {
                result.setZero();
            }

            return result;
        }

        /**
         * Tests if this quaternion is normalized. A normalized quaternion has
         * length of exactly 1.0. However, this method tests the quaternion to have
         * length of 1.0 within a certain margin of error to compensate for floating
         * point imprecision.
         *
         * @return `true` if this quaternion is normalized.
         */
        bool IsNormalized(T epsilon = static_cast<T>(0.000001)) const {
            return MathUtils::IsEqual(length(), (Real) 1.0, epsilon);
        }

        /**
         * Interpolates this quaternion and the specified one and stores the result
         * in this quaternion. This method does a spherical linear interpolation
         * (*SLERP*). Both quaternions must be normalized.
         *
         * @param end
         *            the end quaternion
         * @param t
         *            the interpolation value in the range between [0, 1]
         * @return a reference to this quaternion for method chaining
         */
        Quaternion<T>& Slerp(const Quaternion<T>& end, T t) {
            assert(IsNormalized());
            assert(end.IsNormalized());

            // dot product/cosine of the angle between the two vectors.
            T d = this->w * end.w + this->x * end.x + this->y * end.y + this->z * end.z;
            T absDot = d < 0 ? -d : d;

            T scale0 = 1 - t;
            T scale1 = t;

            // Check if the angle between the 2 quaternions was big enough to
            // warrant such calculations
            if ((1 - absDot) > static_cast<T>(0.1)) {
                // Get the angle between the 2 quaternions,
                // and then store the sin() of that angle
                const Real angle = std::acos(absDot);
                const Real invSinTheta = 1 / std::sin(angle);

                // Calculate the scale for q1 and q2, according to the angle and it's sine value
                scale0 = (std::sin((1 - t) * angle) * invSinTheta);
                scale1 = (std::sin((t * angle)) * invSinTheta);
            }

            if (d < 0.f) scale1 = -scale1;

            // interpolate
            x = (scale0 * x) + (scale1 * end.x);
            y = (scale0 * y) + (scale1 * end.y);
            z = (scale0 * z) + (scale1 * end.z);
            w = (scale0 * w) + (scale1 * end.w);

            NormalizeLazy();
            assert(IsNormalized());

            return *this;
        }

        /**
         * Returns a direct pointer to the memory of this quaternion's elements.
         *
         * @return a pointer to the first element
         */
        T* data() {
            return _data;
        }

        /**
         * Returns a direct pointer to the memory of this quaternion's elements.
         *
         * @return a pointer to the first element
         */
        const T* data() const {
            return _data;
        }

        /**
         * Multiplies this quaternion with the specified quaternion and returns the result.
         *
         * @param right		the right side quaternion
         * @return the result of this operation
         */
        const Quaternion<T> operator*(const Quaternion<T> & right) const {
            T x = this->w * right.x + this->x * right.w + this->y * right.z - this->z * right.y;
            T y = this->w * right.y + this->y * right.w + this->z * right.x - this->x * right.z;
            T z = this->w * right.z + this->z * right.w + this->x * right.y - this->y * right.x;
            T w = this->w * right.w - this->x * right.x - this->y * right.y - this->z * right.z;

            return Quaternion(w, x, y, z);
        }

        /**
         * Multiplies this quaternion with the specified quaternion in-place.
         *
         * @param right		the right side quaternion
         * @return reference to this quaternion
         */
        Quaternion<T> & operator*=(const Quaternion<T> & right) {
            T x = this->w * right.x + this->x * right.w + this->y * right.z - this->z * right.y;
            T y = this->w * right.y + this->y * right.w + this->z * right.x - this->x * right.z;
            T z = this->w * right.z + this->z * right.w + this->x * right.y - this->y * right.x;
            T w = this->w * right.w - this->x * right.x - this->y * right.y - this->z * right.z;

            return Set(w, x, y, z);
        }

        /**
         * Multiplies each component of this quaternion with the specified
         * scalar in-place.
         *
         * @param s		the scalar
         * @return reference to this quaternion
         */
        Quaternion<T> & operator*=(T s) {
            x *= s; y *= s; z *= s; w *= s;
            return *this;
        }

        /**
         * Multiplies each component of this quaternion with the specified
         * scalar and returns the result.
         *
         * @param s		the scalar
         * @return reference to this quaternion
         */
        const Quaternion<T> operator*(T s) const {
            return Quaternion(w * s, x * s, y * s, z * s);
        }

        /**
         * Adds the specified quaternion to this quaternion in-place.
         *
         * @param right		the right side quaternion
         * @return reference to this quaternion
         */
        Quaternion<T> & operator+=(const Quaternion<T> & right) {
            x += right.x; y += right.y; z += right.z; w += right.w;
            return *this;
        }

        /**
         * Adds the specified quaternion to this quaternion and returns the result.
         *
         * @param right		the right side quaternion
         * @return the result of this operation
         */
        const Quaternion<T> operator+(const Quaternion<T> & right) const {
            return Quaternion<T>(w + right.w, x + right.x, y + right.y, z + right.z);
        }

        /**
         * Subtracts the specified quaternion from this quaternion in-place.
         *
         * @param right		the right side quaternion
         * @return reference to this quaternion
         */
        Quaternion<T> & operator-=(const Quaternion<T> & right) {
            x -= right.x; y -= right.y; z -= right.z; w -= right.w;
            return *this;
        }

        /**
         * Subtracts the specified quaternion from this quaternion and returns the result.
         *
         * @param right		the right side quaternion
         * @return the result of this operation
         */
        const Quaternion<T> operator-(const Quaternion<T> & right) const {
            return Quaternion(w - right.w, x - right.x, y - right.y, z - right.z);
        }

        /**
         * Binary equality operator comparing two quaternions.
         *
         * @param the right hand side quaternion
         * @return `true` if the specified quaternion is equal to this quaternion
         */
        bool operator==(const Quaternion<T>& o) const {
            return x == o.x && y == o.y && z == o.y && w == o.w;
        }

        /**
         *  Binary non-equality operator comparing two quaternions.
         *
         * @param the right hand side quaternion
         * @return `true` if the specified quaternion is not equal to this quaternion
         */
        bool operator!=(const Quaternion<T>& o) const {
            return !(*this == o);
        }

        // the quaternion elements
        union
        {
            struct
            {
                /** X-Coordinate of this vector. */
                T x;

                /** Y-Coordinate of this vector. */
                T y;

                /** Z-Coordinate of this vector. */
                T z;

                /** The w-component of this quaternion. */
                T w;
            };

            /** The quaternion data as array of Real values. */
            T _data[4];
        };
    };

    template <typename T>
    inline const Quaternion<T> operator*(T s, const Quaternion<T>& q) {
        return q * s;
    }

    /**
     * Convenient type alias for astu::Matrix4 template using double as data type.
     */
    using Quaternion4d = astu::Quaternion<double>;

    /**
     * Convenient type alias for astu::Matrix4 template using float as data type.
     */
    using Quaternion4f = astu::Quaternion<float>;

} // end of namespace