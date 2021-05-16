/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard library includes
#include <cassert>
#include <utility>
#include <cmath>
#include <iostream>
#include <limits>

// Local includes
#include "Vector2.h"
#include "MathUtils.h"

namespace astu {

	/**
	 * A column-major order 3x3 matrix.
	 * 
     * @ingroup math_group
	 */
    template <typename T>
    class Matrix3 {
    public:

		/**
		 * Constructor.
		 * 
		 * The matrix will be set to the identity.
		 */
		Matrix3()
		{
            SetToIdentity();
		}    

		/**
		 * Constructor.
		 * 
		 * Initializes the matrix with the specified values.
		 *
		 * @param m00 the first value
		 * @param m01 the second value
		 * @param m02 the third value
		 * @param m03 the fourth value
		 * @param m04 the fifth value
		 * @param m05 the sixth value
		 * @param m06 the seventh value
		 * @param m07 the eighth value
		 * @param m08 the ninth value
		 * @param m09 the tenth value
		 */
		Matrix3(T m0, T m1, T m2,
				 T m3, T m4, T m5,
				 T m6, T m7, T m8)
		{
			set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
		}

		/**
		 * Sets this matrix to the specified values.
		 *
		 * @param m00 the first value
		 * @param m01 the second value
		 * @param m02 the third value
		 * @param m03 the fourth value
		 * @param m04 the fifth value
		 * @param m05 the sixth value
		 * @param m06 the seventh value
		 * @param m07 the eighth value
		 * @param m08 the ninth value
		 * @param m09 the tenth value
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& set(T m0, T m1, T m2,
					    T m3, T m4, T m5,
					    T m6, T m7, T m8)
		{
			m[0] = m0;  m[1] = m1;  m[2] = m2;
			m[3] = m3;  m[4] = m4;  m[5] = m5;
			m[6] = m6;  m[7] = m7;  m[8] = m8;
			return *this;
		}

		/**
		 * Sets this matrix to identity.
		 *
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& SetToIdentity() {
			m[0] = 1; m[1] = 0; m[2] = 0;
			m[3] = 0; m[4] = 1; m[5] = 0;
			m[6] = 0; m[7] = 0;	m[8] = 1;
			return *this;
		}

		/**
		 * Sets this matrix to a translation matrix with specified translation.
		 *
		 * @param tx	the distance to translate in the x-axis direction
		 * @param ty	the distance to translate in the y-axis direction
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& SetToTranslate(T tx, T ty) {
			m[0] = 1;  m[1] = 0;  m[2] = 0;
			m[3] = 0;  m[4] = 1;  m[5] = 0;
			m[6] = tx; m[7] = ty; m[8] = 1;
			return *this;
		}

		/**
		 * Sets this matrix to a translation matrix with specified translation.
		 *
		 * @param v		the translation vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& SetToTranslate(const Vector2<T>& v) {
			return SetToTranslate(v.x, v.y);
		}        

		/**
		 * Applies a translation by the specified translation vector.
		 *
		 * This method pre-multiplies a translation matrix 'Mt' with this
		 * matrix and stores the result in this matrix.
		 *
		 * @param v	the translation vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& Translate(T tx, T ty) {
			m[0] += m[2] * tx;  m[3] += m[5] * tx;  m[6] += m[8] * tx;
			m[1] += m[2] * ty;  m[4] += m[5] * ty;  m[7] += m[8] * ty;
			return *this;
		}        

		/**
		 * Applies a translation by the specified translation vector.
		 *
		 * This method pre-multiplies a translation matrix 'Mt' with this
		 * matrix and stores the result in this matrix.
		 *
		 * @param tx	x-component of the translation vector
		 * @param ty	y-component of the translation vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& Translate(const Vector2<T> & v) {
			return Translate(v.x, v.y);
		}        

		/**
		 * Sets this matrix to a scale matrix with the specified scaling factors.
		 *
		 * @param sx	scaling factor for the x-axis
		 * @param sy	scaling factor for the y-axis
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& SetToScale(T sx, T sy) {
			m[0] = sx;  m[1] = 0;   m[2] = 0;
			m[3] = 0;   m[4] = sy;  m[5] = 0;
			m[6] = 0;   m[7] = 0;   m[8] = 1;
			return *this;
		}

		/**
		 * Sets this matrix to a scale matrix with the specified scaling factors.
		 *
		 * @param v		the scaling vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& SetToScale(const Vector2<T> & v) {
			return SetToScale(v.x, v.y);
		}        

		/**
		 * Applies a scaling by the specified scaling factors.
		 *
		 * This method pre-multiplies a scaling matrix 'Ms' with this
		 * matrix and stores the result in this matrix.
		 *
		 * @param sx	scaling factor along the x-axis
		 * @param sy	scaling factor along the y-axis
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T>& Scale(T sx, T sy) {
			m[0] *= sx; m[3] *= sx; m[6] *= sx;
			m[1] *= sy; m[4] *= sy; m[7] *= sy;
			return *this;
		}

		/**
		 * Applies a scaling by the specified scaling vector.
		 *
		 * This method pre-multiplies a scaling matrix 'Ms' with this
		 * matrix and stores the result in this matrix.
		 *
		 * @param v	the scaling vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T> & Scale(const Vector2<T> & v) {
			return Scale(v.x, v.y);
		}

		/**
		 * Sets this matrix to a rotate matrix with the specified angle.
		 *
		 * @param phi	rotation angle in radians
		 * @return reference to this matrix for method chaining
		 */
        Matrix3<T> & SetToRotate(T phi) {
            if (phi == 0.0f) {
                SetToIdentity();
                return *this;
            }
            T sinPhi = std::sin(phi);
            T cosPhi = std::cos(phi);

            m[0] = cosPhi;   m[1] = sinPhi;  m[2] = 0;
            m[3] = -sinPhi;  m[4] = cosPhi;  m[5] = 0;
            m[6] = 0;        m[7] = 0;       m[8] = 1;
    		return *this;
        }     

		/**
		 * Sets this matrix to a rotate matrix with the specified angle.
		 *
		 * @param phi	rotation angle in degrees
		 * @return reference to this matrix for method chaining
		 */
		Matrix3<T> & SetToRotateDeg(T phi) {
			return SetToRotate(ToRadians<T>(phi));
		}

		/**
		 * Applies a counter-clock wise rotation by the specified angle.
		 * This method creates a rotation matrix M<sub>r</sub> and pre-multiplies the
		 * rotation matrix with this matrix: M = M<sub>r</sub> &middot; M.
		 *
		 * @param phi the angle in radians
		 * @return reference to this matrix for method chaining
		 */
        Matrix3<T>& Rotate(T phi) {
            Matrix3<T> tmp;
            *this = tmp.SetToRotate(phi) * *this;

            return *this;
        } 

		/**
		 * Applies a counter-clock wise rotation by the specified angle.
		 * This method creates a rotation matrix M<sub>r</sub> and pre-multiplies the
		 * rotation matrix with this matrix: M = M<sub>r</sub> &middot; M.
		 *
		 * @param phi	rotation angle in degrees
		 * @return reference to this matrix for method chaining
		 */
        Matrix3<T>& RotateDeg(T phi) {
			return Rotate(ToRadians<T>(phi));
        }

		/**
		 * Transforms the specified point.
		 *
		 * The specified vector is considered to be a row vector and hence
		 * multiplied from the right side. The missing fourth element
		 * of the required four component vector is assumed to be one.
		 *
		 * @param p	the point to transform
		 * @return the transformed point
		 */
		Vector2<T> TransformPoint(T x, T y) const {
			return Vector2<T>(
				m[0] * x + m[3] * y + m[6],
				m[1] * x + m[4] * y + m[7]
			);
		}

		/**
		 * Transforms the specified point.
		 *
		 * The specified vector is considered to be a row vector and hence
		 * multiplied from the right side. The missing fourth element
		 * of the required four component vector is assumed to be one.
		 *
		 * @param x	the x-coordinate of the point
		 * @param y	the y-coordinate of the point
		 * @return the transformed point
		 */
		Vector2<T> TransformPoint(const Vector2<T>& p) const {
            return TransformPoint(p.x, p.y);
		}

		/**
		 * Transforms the specified row vector.
		 *
		 * The specified vector is considered to be a row vector and hence
		 * multiplied from the right side. The missing fourth element
		 * of the required four component vector is assumed to be zero.
		 *
		 * @param v	the vector to transform
		 * @return the transformed point
		 */
		Vector2<T> TransformVector(T x, T y) const {
			return Vector2<T>(
				m[0] * x + m[3] * y,
				m[1] * x + m[4] * y
			);
		}        

		/**
		 * Transforms the specified row vector.
		 *
		 * The specified vector is considered to be a row vector and hence
		 * multiplied from the right side. The missing fourth element
		 * of the required four component vector is assumed to be zero.
		 *
		 * @param x	the x-coordinate of the vector
		 * @param y	the y-coordinate of the vector
		 * @return the transformed point
		 */
		Vector2<T> TransformVector(const Vector2<T> & v) const {
			return TransformVector(v.x, v.y);
		}        

		/**
		 * Transposes this matrix.
		 *
		 * @return reference to this matrix for method chaining
		 */
        Matrix3<T>& Transpose() {
            std::swap(m[1], m[3]);
            std::swap(m[2], m[6]);
            std::swap(m[5], m[7]);

            return *this;
        }        

		/**
		 * Inverts this matrix.
		 * 
		 * If no inverse is possible, the matrix is set to identity matrix.
		 *
		 * @return reference to this (now inverted) matrix
		 */
		Matrix3<T>& Invert() {
			T tmp[9];

			tmp[0] = m[4] * m[8] - m[5] * m[7];
			tmp[1] = m[7] * m[2] - m[8] * m[1];
			tmp[2] = m[1] * m[5] - m[2] * m[4];
			tmp[3] = m[5] * m[6] - m[3] * m[8];
			tmp[4] = m[0] * m[8] - m[2] * m[6];
			tmp[5] = m[2] * m[3] - m[0] * m[5];
			tmp[6] = m[3] * m[7] - m[4] * m[6];
			tmp[7] = m[6] * m[1] - m[7] * m[0];
			tmp[8] = m[0] * m[4] - m[1] * m[3];

			T determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
			if (abs(determinant) < std::numeric_limits<T>::min()) {
				SetToIdentity();
			}

			T invDeterminant = 1 / determinant;
			m[0] = invDeterminant * tmp[0];
			m[1] = invDeterminant * tmp[1];
			m[2] = invDeterminant * tmp[2];
			m[3] = invDeterminant * tmp[3];
			m[4] = invDeterminant * tmp[4];
			m[5] = invDeterminant * tmp[5];
			m[6] = invDeterminant * tmp[6];
			m[7] = invDeterminant * tmp[7];
			m[8] = invDeterminant * tmp[8];

			return *this;
		}

		/**
		 * Binary multiply operator for matrices. This operator multiplies this matrix
		 * with the right-hand side matrix.
		 *
		 * @param rhs the right-hand side
		 * @return the result matrix
		 */
		const Matrix3<T> operator *(const Matrix3<T> & rhs) const {
			return Matrix3<T>(
						m[0] * rhs.m[0] + m[3] * rhs.m[1] + m[6] * rhs.m[2],
						m[1] * rhs.m[0] + m[4] * rhs.m[1] + m[7] * rhs.m[2],
						m[2] * rhs.m[0] + m[5] * rhs.m[1] + m[8] * rhs.m[2],

						m[0] * rhs.m[3] + m[3] * rhs.m[4] + m[6] * rhs.m[5],
						m[1] * rhs.m[3] + m[4] * rhs.m[4] + m[7] * rhs.m[5],
						m[2] * rhs.m[3] + m[5] * rhs.m[4] + m[8] * rhs.m[5],

						m[0] * rhs.m[6] + m[3] * rhs.m[7] + m[6] * rhs.m[8],
						m[1] * rhs.m[6] + m[4] * rhs.m[7] + m[7] * rhs.m[8],
						m[2] * rhs.m[6] + m[5] * rhs.m[7] + m[8] * rhs.m[8]);
		}        

		/**
		 * Compound assignment and multiply operator for matrices.
		 * This operator multiplies this matrix with the right-hand side
		 * matrix and stores the result in this matrix.
		 *
		 * @param rhs the right-hand side
		 * @return reference to this matrix
		 */
		Matrix3<T>& operator *=(const Matrix3<T> & rhs) {
			*this = *this * rhs;
			return *this;
		}

		/**
		 * Binary equality operator comparing two matrices.
		 *
		 * @param the right hand side matrix
		 * @return `true` if the specified matrix is equal to this matrix
		 */
		bool operator==(const Matrix3<T>& rhs) const {
			return 
				(m[0] == rhs.m[0]) && (m[1] == rhs.m[1]) && (m[2] == rhs.m[2]) &&
				(m[3] == rhs.m[3]) && (m[4] == rhs.m[4]) && (m[5] == rhs.m[5]) &&
				(m[6] == rhs.m[6]) && (m[7] == rhs.m[7]) && (m[8] == rhs.m[8]);
		}        

		/**
		 * Binary non-equality operator comparing two matrices.
		 *
		 * @param the right hand side matrix
		 * @return `true` if the specified matrix is not equal to this matrix
		 */
		bool operator!=(const Matrix3<T> rhs) const {
			return !(*this == rhs);
		}

		/**
		 * Subscript operator for this matrix. The specified index must be
		 * within the range [0, 8].
		 *
		 * @param idx	index of the matrix value to be retrieved
		 * @return the requested value
		 */
		T& operator[](size_t idx)
		{
			assert (idx <= 8);
			return m[idx];
		}

		/**
		 * Subscript operator for this matrix. The specified index must be
		 * within the range [0, 8].
		 *
		 * @param idx	index of the matrix value to be retrieved
		 * @return the requested value
		 */
		const T& operator[](size_t idx) const
		{
			assert (idx <= 8);
			return m[idx];
		}        

    private:
        T m[9];
    };

	/**
	 * Convenient type alias for astu::Matrix3 template using double as data type.
	 */
	using Matrix3d = astu::Matrix3<double>;

	/**
	 * Convenient type alias for astu::Matrix3 template using float as data type.
	 */
	using Matrix3f = astu::Matrix3<float>;

} // end of namespace



// std::ostream & operator<<(std::ostream& os, const astu::Matrix3& mat) {
// 	os << '[';
// 	os << mat[0] << ", " << mat[3] << ", " << mat[6];
// 	os << ", ";
// 	os << mat[1] << ", " << mat[4] << ", " << mat[7];
// 	os << ", ";
// 	os << mat[2] << ", " << mat[5] << ", " << mat[8];
// 	os << ']';

// 	return os;
// }
