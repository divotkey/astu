/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Vector3.h"
#include "Quaternion.h"
#include "MathUtils.h"

// C++ Standard library includes
#include <cassert>
#include <iostream>

namespace astu {

	/**
	 * A column-major order 4x4 matrix.
	 * The matrix elements are stored as 16 contiguous floating-point values
	 * with the 13th (index 12) , 14th (index 13), and 15th (index 14)
	 * elements representing the X, Y, and Z, translation components.
	 *
     * @ingroup math_group
	 */
    template<typename T>
	class Matrix4 {
	public:

		/** The Identity matrix. */
        static const Matrix4<T> Identity;

        /**
         * Creates a viewing matrix derived from an eye point, a target point, and an UP vector.
         *
         * @param eye       the position of the observer (camera etc.)
         * @param center    the target the observer should look at
         * @param up        the up direction of the observer
         * @return the newly created view matrix
         * @param tparam    the numerical type of this matrix
         */
        static Matrix4<T> CreateLookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up) {
            //TODO optimize and beautify LookAt method
            Vector3<T> f = center - eye;
            f.Normalize();

            Vector3<T> s(f);
            s.Cross(up).Normalize();

            Vector3<T> u(s);
            u.Cross(f);

            Matrix4<T> result( s.x,  u.x, -f.x, 0,
                               s.y,  u.y, -f.y, 0,
                               s.z,  u.z, -f.z, 0,
                               0,    0,    0, 1);

            Matrix4<T> tx;
            tx.SetToTranslate(-eye);
            result *= tx;
            return result;
        }

        /**
         * Creates up a perspective projection matrix.
         *
         * @param near      the distance from the viewer to the near clipping plane
         * @param far       the distance from the viewer to the far clipping plane
         * @param fovy      specifies the field of view angle, in degrees, in the y direction
         * @param aspect    specifies the aspect ratio that determines the field of view in the x direction
         * @return the newly created perspective projection matrix
         * @param tparam    the numerical type of this matrix
         */
        static Matrix4<T> CreatePerspective(T near, T far, T fovy, T aspect)
        {
            assert(near > 0);
            assert(far > near);

            //T fd = 1.0 / tan((fovy * (Math.PI / 180)) / 2.0);
            T fd = 1.0 / tan(MathUtils::ToRadians<T>(fovy) / 2.0);
            T a1 = (far + near) / (near - far);
            T a2 = (2 * far * near) / (near - far);

            return Matrix4<T>(fd / aspect,   0,   0,  0,
                              0,  fd,   0,  0,
                              0,   0,  a1, -1,
                              0,   0,  a2,  0);
        }

		/**
		 * Constructor.
		 *
		 * The matrix will be set to the identity.
		 */
		Matrix4()
		{
            SetToIdentity();
		}

		/**
		 * Constructor. Initializes the matrix with the specified values.
		 *
		 * @param m00   the first element of the first column
		 * @param m01   the second element of the first column
		 * @param m02   the third element of the first column
		 * @param m03   the fourth element of th first column
		 * @param m04   the first element of the second column
		 * @param m05   the second element of the second column
		 * @param m06   the third element of the second column
		 * @param m07   the fourth element of the second column
		 * @param m08   the first element of the third column
		 * @param m09   the second element of the third column
		 * @param m10   the third element of the third column
		 * @param m11   the fourth element of the third column
		 * @param m12   the first element of the fourth column
		 * @param m13   the second element of the fourth column
		 * @param m14   the third element of the fourth column
		 * @param m15   the fourth element of the fourth column
		 */
		Matrix4(
			T m00, T m01, T m02, T m03,
			T m04, T m05, T m06, T m07,
			T m08, T m09, T m10, T m11,
			T m12, T m13, T m14, T m15)
		{
			m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
			m[4] = m04;  m[5] = m05;  m[6] = m06;  m[7] = m07;
			m[8] = m08;  m[9] = m09;  m[10] = m10;  m[11] = m11;
			m[12] = m12;  m[13] = m13;  m[14] = m14;  m[15] = m15;
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
		 * @param m10 the eleventh value
		 * @param m11 the twelfth value
		 * @param m12 the thirteenth value
		 * @param m13 the fourteenth vlaue
		 * @param m14 the fifteenth value
		 * @param m15 the sixteenth value
		 *
		 * @return reference to this matrix for method chaining
		 */
		Matrix4 & Set(
			T m00, T m01, T m02, T m03,
			T m04, T m05, T m06, T m07,
			T m08, T m09, T m10, T m11, 
			T m12, T m13, T m14, T m15)
		{
			m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
			m[4] = m04;  m[5] = m05;  m[6] = m06;  m[7] = m07;
			m[8] = m08;  m[9] = m09;  m[10] = m10;  m[11] = m11;
			m[12] = m12;  m[13] = m13;  m[14] = m14;  m[15] = m15;
			return *this;
		}

		/**
		 * Constructor.
		 *
		 * Initializes the matrix with the specified values
		 *
		 * @param data array containing the matrix values
		 */
		Matrix4(const T data[16]) {
			m[0] = data[0];  m[1] = data[1];  m[2] = data[2];  m[3] = data[3];
			m[4] = data[4];  m[5] = data[5];  m[6] = data[6];  m[7] = data[7];
			m[8] = data[8];  m[9] = data[9];  m[10] = data[10]; m[11] = data[11];
			m[12] = data[12]; m[13] = data[13]; m[14] = data[14]; m[15] = data[15];
		}

		/**
		 * Sets this matrix to the specified values.
		 *
		 * @param data array containing the matrix values
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& Set(const T data[16]) {
			m[0] = data[0];  m[1] = data[1];  m[2] = data[2];  m[3] = data[3];
			m[4] = data[4];  m[5] = data[5];  m[6] = data[6];  m[7] = data[7];
			m[8] = data[8];  m[9] = data[9];  m[10] = data[10]; m[11] = data[11];
			m[12] = data[12]; m[13] = data[13]; m[14] = data[14]; m[15] = data[15];
			return *this;
		}

		/**
		 * Sets this matrix to the specified matrix.
		 *
		 * @param o the source matrix
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& Set(const Matrix4& o) {
			return set(o.m);
		}

		/**
		 * Sets this matrix to identity.
		 *
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& SetToIdentity() {
			m[0] = m[5] = m[10] = m[15] = 1.0f;
			m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
			return *this;
		}

		/**
		 * Sets this matrix to a translation matrix with specified translation.
		 *
		 * @param tx	the distance to translate in the x-axis direction
		 * @param ty	the distance to translate in the y-axis direction
		 * @param tz	the distance to translate in the y-axis direction
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& SetToTranslate(T tx, T ty, T tz) {
			m[0]  =  1;  m[1]  =  0;  m[2]  =  0; m[3]  = 0;
			m[4]  =  0;  m[5]  =  1;  m[6]  =  0; m[7]  = 0;
			m[8]  =  0;  m[9]  =  0;  m[10] =  1; m[11] = 0;
			m[12] = tx;  m[13] = ty;  m[14] = tz; m[15] = 1;
			return *this;
		}

		/**
		 * Sets this matrix to a translation matrix with specified translation.
		 *
		 * @param v		the translation vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& SetToTranslate(const Vector3<T> & v) {
			return SetToTranslate(v.x, v.y, v.z);
		}

        /**
         * Applies a translation by the specified translation vector.
         *
         * This method pre-multiplies a translation matrix 'Mt' with this
         * matrix and stores the result in this matrix.
         *
         * @param t the translation vector
         * @return reference to this matrix for method chaining
         */
        Matrix4& Translate(const Vector3<T>& t) {
            return Translate(t.x, t.y, t.y);
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
		 * @param tz	y-component of the translation vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& Translate(T tx, T ty, T tz) {
			m[0] += m[3] * tx;   m[4] += m[7] * tx;   m[8]  += m[11] * tx;   m[12] += m[15] * tx;
			m[1] += m[3] * ty;   m[5] += m[7] * ty;   m[9]  += m[11] * ty;   m[13] += m[15] * ty;
			m[2] += m[3] * tz;   m[6] += m[7] * tz;   m[10] += m[11] * tz;   m[14] += m[15] * tz;

			return *this;
		}

		/**
		 * Sets this matrix to a scale matrix with the specified scaling factors.
		 *
		 * @param sx	scaling factor along the x-axis
		 * @param sy	scaling factor along the y-axis
		 * @param sz	scaling factor along the z-axis
		 * @return reference to this matrix for method chaining
		 */
		Matrix4 & SetToScale(T sx, T sy, T sz) {
			m[0]  = sx;  m[1] =  0;   m[2]  =  0; m[3]  = 0;
			m[4]  =  0;  m[5] = sy;   m[6]  =  0; m[7]  = 0;
			m[8]  =  0;  m[9] =  0;   m[10] = sz; m[11] = 0;
			m[12] =  0;  m[13] =  0;  m[14] =  0; m[15] = 1;
			return *this;
		}

		/**
		 * Sets this matrix to a scale matrix with the specified scaling factors.
		 *
		 * @param v		the scaling vector
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& SetToScale(const Vector3<T> & v) {
			return SetToScale(v.x, v.y, v.z);
		}

		/**
		 * Applies a scaling by the specified scaling factors.
		 *
		 * This method pre-multiplies a scaling matrix 'Ms' with this
		 * matrix and stores the result in this matrix.
		 *
		 * @param sx	scaling factor along the x-axis
		 * @param sy	scaling factor along the y-axis
		 * @param sz	scaling factor along the z-axis
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& Scale(T sx, T sy, T sz) {
			m[0] *= sx; m[4] *= sx; m[8]  *= sx; m[12] *= sx;
			m[1] *= sy; m[5] *= sy; m[9]  *= sy; m[13] *= sy;
			m[2] *= sz; m[6] *= sz; m[10] *= sz; m[14] *= sz;
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
		Matrix4& Scale(const Vector3<T>& v) {
			return scale(v.x, v.y, v.z);
		}

        /**
         * Sets this matrix to a rotation matrix rotating about the x-axis by the specified angle.
         *
         * @param phi   the rotation angle in radians
		 * @return reference to this matrix for method chaining
         */
        Matrix4& SetToRotationX(T phi) {
            T sin = std::sin(phi); T cos = std::cos(phi);
            m[0] = 1;   m[4] = 0;   m[ 8] = 0;    m[12] = 0;
            m[1] = 0;   m[5] = cos; m[ 9] = -sin; m[13] = 0;
            m[2] = 0;   m[6] = sin; m[10] = cos;  m[14] = 0;
            m[3] = 0;   m[7] = 0;   m[11] = 0;    m[15] = 1;
            return *this;
        }

        /**
         * Sets this matrix to a rotation matrix rotating about the y-axis by the specified angle.
         *
         * @param phi   the rotation angle in radians
		 * @return reference to this matrix for method chaining
         */
        Matrix4& SetToRotationY(T phi) {
            T sin = std::sin(phi); T cos = std::cos(phi);

            m[0] = cos;     m[4] = 0;   m[ 8] = sin;    m[12] = 0;
            m[1] = 0;       m[5] = 1;   m[ 9] = 0;      m[13] = 0;
            m[2] = -sin;    m[6] = 0;   m[10] = cos;    m[14] = 0;
            m[3] = 0;       m[7] = 0;   m[11] = 0;      m[15] = 1;
            return *this;
        }

		/**
		 * Applies a rotation about the x-asis by the specified angle.
		 *
		 * This method pre-multiplies a rotation matrix 'Mr' with this
		 * matrix and stores the result in this matrix.
		 *
         * @param phi   the rotation angle in radians
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& RotateX(T phi) {
			return *this = Matrix4().SetToRotationX(phi) * *this;
		}

		/**
		 * Applies a rotation about the y-asis by the specified angle.
		 *
		 * This method pre-multiplies a rotation matrix 'Mr' with this
		 * matrix and stores the result in this matrix.
		 *
         * @param phi   the rotation angle in radians
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& RotateY(T phi) {
			return *this = Matrix4().SetToRotationY(phi) * *this;
		}

		/**
		 * Applies a rotation about the z-asis by the specified angle.
		 *
		 * This method pre-multiplies a rotation matrix 'Mr' with this
		 * matrix and stores the result in this matrix.
		 *
         * @param phi   the rotation angle in radians
		 * @return reference to this matrix for method chaining
		 */
		Matrix4& RotateZ(T phi) {
			return *this = Matrix4().SetToRotationZ(phi) * *this;
		}

		/**
         * Sets this matrix to a rotation matrix rotating about the z-axis by the specified angle.
         *
         * @param phi   the rotation angle in radians
		 * @return reference to this matrix for method chaining
         */
        Matrix4& SetToRotationZ(T phi) {
            T sin = std::sin(phi); T cos = std::cos(phi);

            m[0] = cos; m[4] = -sin;    m[ 8] = 0;  m[12] = 0;
            m[1] = sin; m[5] = cos;     m[ 9] = 0;  m[13] = 0;
            m[2] = 0;   m[6] = 0;       m[10] = 1;  m[14] = 0;
            m[3] = 0;   m[7] = 0;       m[11] = 0;  m[15] = 1;
            return *this;
        }

        /**
         * Sets this  matrix to the rotation defined by the given quaternion.
         * @param q     the quaternion defining the rotation
         * @return reference to this matrix for method chaining
         */
        Matrix4& SetToRotation(const Quaternion<T>& q) {
            return Set(0, 0, 0, q.w, q.x, q.y, q.z);
        }

        /**
         * Sets this matrix to the transformation defined by the given vector and quaternion.
         *
         * @param t the position / translation
         * @param q the orientation
         * @return reference to this matrix for method chaining
         */
        Matrix4& Set(const Vector3<T>& t, const Quaternion<T>& q) {
            return set(t.x, t.y, t.z, q.w, q.x, q.y, q.z);
        }

        /**
         * Sets this matrix to the transformation defined by the given vector and quaternion.
         *
         * @param tx    the x-component of the position vector
         * @param ty    the y-component of the position vector
         * @param tz    the z-component of the position vector
         * @param qw    the w-component of the orientation quaternion
         * @param qx    the x-component of the orientation quaternion
         * @param qy    the y-component of the orientation quaternion
         * @param qz    the z-component of the orientation quaternion
         * @return reference to this matrix for method chaining
         */
        Matrix4& Set(T tx, T ty, T tz, T qw, T qx, T qy, T qz) {
            T xs = qx * 2,  ys = qy * 2,  zs = qz * 2;
            T wx = qw * xs, wy = qw * ys, wz = qw * zs;
            T xx = qx * xs, xy = qx * ys, xz = qx * zs;
            T yy = qy * ys, yz = qy * zs, zz = qz * zs;

            m[0]    = (1.0f - (yy + zz));
            m[4]    = (xy - wz);
            m[8]    = (xz + wy);
            m[12]   = tx;

            m[1]    = (xy + wz);
            m[5]    = (1.0f - (xx + zz));
            m[9]    = (yz - wx);
            m[13]   = ty;

            m[2]    = (xz - wy);
            m[6]    = (yz + wx);
            m[10]   = (1.0f - (xx + yy));
            m[14]   = tz;

            m[3]    = 0.f;
            m[7]    = 0.f;
            m[11]   = 0.f;
            m[15]   = 1.0f;

            return *this;
        }

		/**
		 * Transposes this matrix.
		 *
		 * @return reference to this matrix for method chaining
		 */
		Matrix4<T>& Transpose() {
			std::swap(m[1], m[4]);
			std::swap(m[2], m[8]);
			std::swap(m[6], m[9]);
			std::swap(m[3], m[12]);
			std::swap(m[7], m[13]);
			std::swap(m[11], m[14]);

			return *this;
		}

		/**
		 * Inverts this matrix.
		 *
		 * If no inverse is possible, the matrix is set to identity matrix.
		 *
		 * @return reference to this (now inverted) matrix
		 */
		Matrix4<T>& Invert() {
			T tmp[16];

			tmp[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] +
					 m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			tmp[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] -
					 m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			tmp[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] +
					 m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			tmp[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] -
					  m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			tmp[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] -
					 m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			tmp[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] +
					 m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			tmp[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] -
					 m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			tmp[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] +
					  m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			tmp[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] +
					 m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
			tmp[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] -
					 m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
			tmp[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] +
					  m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
			tmp[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] -
					  m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
			tmp[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] -
					 m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
			tmp[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] +
					 m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
			tmp[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] -
					  m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
			tmp[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] +
					  m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

			T determinant = m[0] * tmp[0] + m[1] * tmp[4] + m[2] * tmp[8] + m[3] * tmp[12];

			if (abs(determinant) < std::numeric_limits<T>::min()) {
				SetToIdentity();
			} else {
				T invDeterminant = static_cast<T>(1) / determinant;
				m[0] = tmp[0] * invDeterminant;
				m[1] = tmp[1] * invDeterminant;
				m[2] = tmp[2] * invDeterminant;
				m[3] = tmp[3] * invDeterminant;
				m[4] = tmp[4] * invDeterminant;
				m[5] = tmp[5] * invDeterminant;
				m[6] = tmp[6] * invDeterminant;
				m[7] = tmp[7] * invDeterminant;
				m[8] = tmp[8] * invDeterminant;
				m[9] = tmp[9] * invDeterminant;
				m[10] = tmp[10] * invDeterminant;
				m[11] = tmp[11] * invDeterminant;
				m[12] = tmp[12] * invDeterminant;
				m[13] = tmp[13] * invDeterminant;
				m[14] = tmp[14] * invDeterminant;
				m[15] = tmp[15] * invDeterminant;
			}

			return *this;
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
		Vector3<T> TransformPoint(const Vector3<T>& p) const {
			return Vector3(
				p.x * m[0] + p.y * m[4] + p.z * m[8] + m[12], 
				p.x * m[1] + p.y * m[5] + p.z * m[9] + m[13], 
				p.x * m[2] + p.y * m[6] + p.z * m[10] + m[14]
				);
		}

		/**
		 * Transforms the specified point.
		 * The result will be stored in the specified result vector.
		 *
		 * The specified vector is considered to be a row vector and hence
		 * multiplied from the right side. The missing fourth element
		 * of the required four component vector is assumed to be one.
		 *
		 * @param p		the point to transform
		 * @param pt	the point where to store the result
		 * @return reference to the specified result vector
		 */
		Vector3<T>& TransformPoint(const Vector3<T>& p, Vector3<T>& pt) const {
			return pt.Set(
				p.x * m[0] + p.y * m[4] + p.z * m[8] + m[12],
				p.x * m[1] + p.y * m[5] + p.z * m[9] + m[13],
				p.x * m[2] + p.y * m[6] + p.z * m[10] + m[14]
			);
		}

		/**
		 * Transforms the specified row vector.
		 *
		 * The specified vector is considered to be a row vector and hence
		 * multiplied from the right side. The missing fourth element
		 * of the required four component vector is assumed to be zero.
		 *
		 * @param v	the vector to transform
		 * @return the transformed vector
		 */
		Vector3<T> TransformVector(const Vector3<T>& v) const {
			return Vector3(
				v.x * m[0] + v.y * m[4] + v.z * m[8],
				v.x * m[1] + v.y * m[5] + v.z * m[9],
				v.x * m[2] + v.y * m[6] + v.z * m[10]
			);
		}

		/**
		 * Transforms the specified row vector.
		 * The result will be stored in the specified result vector.
		 *
		 * The specified vector is considered to be a row vector and hence
		 * multiplied from the right side. The missing fourth element
		 * of the required four component vector is assumed to be zero.
		 *
		 * @param v		the point to transform
		 * @param vt	the point where to store the result
		 * @return reference to the specified result vector
		 */
		Vector3<T>& TransformVector(const Vector3<T>& v, Vector3<T>& vt) const {
			return vt.Set(
				v.x * m[0] + v.y * m[4] + v.z * m[8],
				v.x * m[1] + v.y * m[5] + v.z * m[9],
				v.x * m[2] + v.y * m[6] + v.z * m[10]
			);
		}

        /**
         * Transforms the specified row vector by the transposed of this matrix.
         *
         * This multiplication is carried out as if this matrix has been transposed.
         *
         * The specified vector is considered to be a row vector and hence
         * multiplied from the right side. The missing fourth element
         * of the required four component vector is assumed to be zero.
         *
         * @param v	the vector to transform
         * @return the transformed vector
         */
        Vector3<T> TransformVectorTransposed(const Vector3<T>& v) const {
            return Vector3(
                    v.x * m[0] + v.y * m[1] + v.z * m[2],
                    v.x * m[4] + v.y * m[5] + v.z * m[6],
                    v.x * m[8] + v.y * m[9] + v.z * m[10]
            );
        }

        /**
         * Transforms the specified row vector by the transposed of this matrix.
         *
         * The result will be stored in the specified result vector.
         *
         * This multiplication is carried out as if this matrix has been transposed.
         *
         * The specified vector is considered to be a row vector and hence
         * multiplied from the right side. The missing fourth element
         * of the required four component vector is assumed to be zero.
         *
         * @param v		the point to transform
         * @param vt	the point where to store the result
         * @return reference to the specified result vector
         */
        Vector3<T>& TransformVectorTransposed(const Vector3<T>& v, Vector3<T>& vt) const {
            return vt.Set(
                    v.x * m[0] + v.y * m[1] + v.z * m[2],
                    v.x * m[4] + v.y * m[5] + v.z * m[6],
                    v.x * m[8] + v.y * m[9] + v.z * m[10]
            );
        }


        /**
         * Projects the specified row vector.
         * The vector components are divided by w assuming that the fourth element of the vector is one.
         *
         * @param p the point to project
         * @return the projected point
         */
        Vector3<T> ProjectPoint(const Vector3<T>& p) const {
            T w = p.x * m[3] + p.y * m[7] + p.z * m[11] + m[15];

            return Vector3(
                    (p.x * m[0] + p.y * m[4] + p.z * m[8] + m[12]) / w,
                    (p.x * m[1] + p.y * m[5] + p.z * m[9] + m[13]) / w,
                    (p.x * m[2] + p.y * m[6] + p.z * m[10] + m[14]) / w
            );
        }

		/**
		 * Binary multiply operator for matrices. This operator multiplies this matrix
		 * with the right-hand side matrix and returns the resulting matrix.
		 *
		 * The multiplication of a row vector with the matrix C which is the result 
		 * of a multiplication of two matrices A and B is identical to multiplying
		 * the vector first with matrix B and then with matrix A.
		 *
		 * C = A &middot; B.
		 * C &middot; p = (A &middot; B &middot; p) = B &middot; ((A &middot; p)).
		 *
		 * @param rhs the right-hand side
		 * @return the result matrix
		 */
		const Matrix4<T> operator *(const Matrix4<T>& rhs) const {
			return Matrix4(
				m[0] * rhs.m[0] + m[4] * rhs.m[1] + m[8]  * rhs.m[2] + m[12] * rhs.m[3],
				m[1] * rhs.m[0] + m[5] * rhs.m[1] + m[9]  * rhs.m[2] + m[13] * rhs.m[3],
				m[2] * rhs.m[0] + m[6] * rhs.m[1] + m[10] * rhs.m[2] + m[14] * rhs.m[3],
				m[3] * rhs.m[0] + m[7] * rhs.m[1] + m[11] * rhs.m[2] + m[15] * rhs.m[3],

				m[0] * rhs.m[4] + m[4] * rhs.m[5] + m[8]  * rhs.m[6] + m[12] * rhs.m[7],
				m[1] * rhs.m[4] + m[5] * rhs.m[5] + m[9]  * rhs.m[6] + m[13] * rhs.m[7],
				m[2] * rhs.m[4] + m[6] * rhs.m[5] + m[10] * rhs.m[6] + m[14] * rhs.m[7],
				m[3] * rhs.m[4] + m[7] * rhs.m[5] + m[11] * rhs.m[6] + m[15] * rhs.m[7],

				m[0] * rhs.m[8] + m[4] * rhs.m[9] + m[8]  * rhs.m[10] + m[12] * rhs.m[11],
				m[1] * rhs.m[8] + m[5] * rhs.m[9] + m[9]  * rhs.m[10] + m[13] * rhs.m[11],
				m[2] * rhs.m[8] + m[6] * rhs.m[9] + m[10] * rhs.m[10] + m[14] * rhs.m[11],
				m[3] * rhs.m[8] + m[7] * rhs.m[9] + m[11] * rhs.m[10] + m[15] * rhs.m[11],

				m[0] * rhs.m[12] + m[4] * rhs.m[13] + m[8]  * rhs.m[14] + m[12] * rhs.m[15],
				m[1] * rhs.m[12] + m[5] * rhs.m[13] + m[9]  * rhs.m[14] + m[13] * rhs.m[15],
				m[2] * rhs.m[12] + m[6] * rhs.m[13] + m[10] * rhs.m[14] + m[14] * rhs.m[15],
				m[3] * rhs.m[12] + m[7] * rhs.m[13] + m[11] * rhs.m[14] + m[15] * rhs.m[15]
			);
		}

		/**
		 * Compound assignment and multiply operator for matrices.
		 * This operator multiplies this matrix with the right-hand side
		 * matrix and stores the result in this matrix.
		 *
		 * @param rhs the right-hand side
		 * @return reference to this matrix
		 */
		Matrix4<T>& operator *=(const Matrix4<T> & rhs) {
			*this = *this * rhs;
			return *this;
		}

		/**
		 * Binary multiply operator for row vectors.
		 * The missing fourth element of the required four component vector
		 * is assumed to be one.
		 *
		 * @param rhs the right-hand side vector
		 * @return the result vector
		 */
		const Vector3<T> operator *(const Vector3<T>& rhs) const {
			return TransformPoint(rhs);
		}

		/**
		 * Binary equality operator comparing two matrices.
		 *
		 * @param the right hand side matrix
		 * @return `true` if the specified matrix is equal to this matrix
		 */
		bool operator==(const Matrix4<T>& rhs) const {
			return
				(m[0] == rhs.m[0])   && (m[1] == rhs.m[1])   && (m[2] == rhs.m[2])   && (m[3] == rhs.m[3])   &&
				(m[4] == rhs.m[4])   && (m[5] == rhs.m[5])   && (m[6] == rhs.m[6])   && (m[7] == rhs.m[7])   &&
				(m[8] == rhs.m[8])   && (m[9] == rhs.m[9])   && (m[10] == rhs.m[10]) && (m[11] == rhs.m[11]) &&
				(m[12] == rhs.m[12]) && (m[13] == rhs.m[13]) && (m[14] == rhs.m[14]) && (m[15] == rhs.m[15]);
		}

		/**
		 * Binary non-equality operator comparing two matrices.
		 *
		 * @param the right hand side matrix
		 * @return `true` if the specified matrix is not equal to this matrix
		 */
		bool operator!=(const Matrix4<T>& rhs) const {
			return !(*this == rhs);
		}

		/**
		 * Subscript operator for this matrix. The specified index must be
		 * within the range [0, 15].
		 *
		 * @param idx	index of the matrix value to be retrieved
		 * @return the requested value
		 */
		T& operator[](size_t idx)
		{
			assert(idx <= 15);
			return m[idx];
		}

		/**
		 * Subscript operator for this matrix. The specified index must be
		 * within the range [0, 15].
		 *
		 * @param idx	index of the matrix value to be retrieved
		 * @return the requested value
		 */
		const T& operator[](size_t idx) const
		{
			assert(idx <= 15);
			return m[idx];
		}

		/**
		 * Returns a direct pointer to the memory of this matrix's elements.
		 *
		 * @return a pointer to the element data
		 */
		T* data() {
			return m;
		}

		/**
		 * Returns a direct pointer to the memory of this matrix's elements.
		 *
		 * @return a pointer to the element datta
		 */
		const T* data() const {
			return m;
		}

	private:
		/** The matrix elements. */
		T m[16];
	};

    /**
     * Defines the Identity-Constant für Matrix4 templates.
     * (For some reasons, MS C++ compiler does not work with template inline
     * keyword.)
     *
     * @param tparam    the numerical type of this matrix
     */
    template <typename T>
    Matrix4<T> const Matrix4<T>::Identity = Matrix4<T>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    /**
     * Convenient type alias for astu::Matrix4 template using double as data type.
     */
    using Matrix4d = astu::Matrix4<double>;

    /**
     * Convenient type alias for astu::Matrix4 template using float as data type.
     */
    using Matrix4f = astu::Matrix4<float>;

} // end of namespace

template <typename T>
std::ostream & operator<<(std::ostream& os, const astu::Matrix4<T>& mat) {
    os << '[';
    os << mat[0] << ", " << mat[4] << ", " << mat[8] << ", " << mat[12];
    os << ", ";
    os << mat[1] << ", " << mat[5] << ", " << mat[9] << ", " << mat[13];
    os << ", ";
    os << mat[2] << ", " << mat[6] << ", " << mat[10] << ", " << mat[14];
    os << ", ";
    os << mat[3] << ", " << mat[7] << ", " << mat[11] << ", " << mat[15];
    os << ']';

    return os;
}
