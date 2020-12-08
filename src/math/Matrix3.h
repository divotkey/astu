/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <utility>
#include <cmath>
#include <iostream>
#include <limits>
#include "Vector2.h"

#pragma once

namespace astu {

	/**
	 * A column-major order 3x3 matrix.
	 */
    class Matrix3 {
    public:

		Matrix3()
		{
            SetToIdentity();
		}    

		Matrix3(double m0, double m1, double m2,
				double m3, double m4, double m5,
				double m6, double m7, double m8)
		{
			set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
		}

		Matrix3& set(double m0, double m1, double m2,
					 double m3, double m4, double m5,
					 double m6, double m7, double m8)
		{
			m[0] = m0;  m[1] = m1;  m[2] = m2;
			m[3] = m3;  m[4] = m4;  m[5] = m5;
			m[6] = m6;  m[7] = m7;  m[8] = m8;
			return *this;
		}

		Matrix3& SetToIdentity() {
			m[0] = 1; m[1] = 0; m[2] = 0;
			m[3] = 0; m[4] = 1; m[5] = 0;
			m[6] = 0; m[7] = 0;	m[8] = 1;
			return *this;
		}

		Matrix3& SetToTranslate(double tx, double ty) {
			m[0] = 1;  m[1] = 0;  m[2] = 0;
			m[3] = 0;  m[4] = 1;  m[5] = 0;
			m[6] = tx; m[7] = ty; m[8] = 1;
			return *this;
		}

		Matrix3& SetToTranslate(const Vector2<double>& v) {
			return SetToTranslate(v.x, v.y);
		}        

		Matrix3& Translate(double tx, double ty) {
			m[0] += m[2] * tx;  m[3] += m[5] * tx;  m[6] += m[8] * tx;
			m[1] += m[2] * ty;  m[4] += m[5] * ty;  m[7] += m[8] * ty;
			return *this;
		}        

		Matrix3& Translate(const Vector2<double> & v) {
			return Translate(v.x, v.y);
		}        

		Matrix3& SetToScale(double sx, double sy) {
			m[0] = sx;  m[1] = 0;   m[2] = 0;
			m[3] = 0;   m[4] = sy;  m[5] = 0;
			m[6] = 0;   m[7] = 0;   m[8] = 1;
			return *this;
		}

		Matrix3& SetToScale(const Vector2<double> & v) {
			return SetToScale(v.x, v.y);
		}        

		Matrix3& Scale(double sx, double sy) {
			m[0] *= sx; m[3] *= sx; m[6] *= sx;
			m[1] *= sy; m[4] *= sy; m[7] *= sy;
			return *this;
		}

		Matrix3 & Scale(const Vector2<double> & v) {
			return Scale(v.x, v.y);
		}

        Matrix3 & SetToRotate(double phi) {
            if (phi == 0.0f) {
                SetToIdentity();
                return *this;
            }
            double sinPhi = std::sin(phi);
            double cosPhi = std::cos(phi);

            m[0] = cosPhi;   m[1] = sinPhi;  m[2] = 0;
            m[3] = -sinPhi;  m[4] = cosPhi;  m[5] = 0;
            m[6] = 0;        m[7] = 0;       m[8] = 1;
    		return *this;
        }     

        Matrix3& Rotate(double phi) {
            Matrix3 tmp;
            *this = tmp.SetToRotate(phi) * *this;

            return *this;
        }        

		Vector2<double> transformPoint(double x, double y) const {
			return Vector2<double>(
				m[0] * x + m[3] * y + m[6],
				m[1] * x + m[4] * y + m[7]
			);
		}

		Vector2<double> TransformPoint(const Vector2<double>& p) const {
            return transformPoint(p.x, p.y);
		}

		Vector2<double> TransformVector(double x, double y) const {
			return Vector2<double>(
				m[0] * x + m[3] * y,
				m[1] * x + m[4] * y
			);
		}        

		Vector2<double> TransformVector(const Vector2<double> & v) const {
			return TransformVector(v.x, v.y);
		}        

        Matrix3& Transpose() {
            std::swap(m[1], m[3]);
            std::swap(m[2], m[6]);
            std::swap(m[5], m[7]);

            return *this;
        }        

		Matrix3& Invert() {
			double tmp[9];

			tmp[0] = m[4] * m[8] - m[5] * m[7];
			tmp[1] = m[7] * m[2] - m[8] * m[1];
			tmp[2] = m[1] * m[5] - m[2] * m[4];
			tmp[3] = m[5] * m[6] - m[3] * m[8];
			tmp[4] = m[0] * m[8] - m[2] * m[6];
			tmp[5] = m[2] * m[3] - m[0] * m[5];
			tmp[6] = m[3] * m[7] - m[4] * m[6];
			tmp[7] = m[6] * m[1] - m[7] * m[0];
			tmp[8] = m[0] * m[4] - m[1] * m[3];

			double determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
			if (abs(determinant) < std::numeric_limits<double>::min()) {
				SetToIdentity();
			}

			double invDeterminant = 1.0f / determinant;
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

		const Matrix3 operator *(const Matrix3 & rhs) const {
			return Matrix3(m[0] * rhs.m[0] + m[3] * rhs.m[1] + m[6] * rhs.m[2],
						   m[1] * rhs.m[0] + m[4] * rhs.m[1] + m[7] * rhs.m[2],
						   m[2] * rhs.m[0] + m[5] * rhs.m[1] + m[8] * rhs.m[2],

						   m[0] * rhs.m[3] + m[3] * rhs.m[4] + m[6] * rhs.m[5],
						   m[1] * rhs.m[3] + m[4] * rhs.m[4] + m[7] * rhs.m[5],
						   m[2] * rhs.m[3] + m[5] * rhs.m[4] + m[8] * rhs.m[5],

						   m[0] * rhs.m[6] + m[3] * rhs.m[7] + m[6] * rhs.m[8],
						   m[1] * rhs.m[6] + m[4] * rhs.m[7] + m[7] * rhs.m[8],
						   m[2] * rhs.m[6] + m[5] * rhs.m[7] + m[8] * rhs.m[8]);
		}        

		Matrix3& operator *=(const Matrix3 & rhs) {
			*this = *this * rhs;
			return *this;
		}

		bool operator==(const Matrix3& rhs) const {
			return 
				(m[0] == rhs.m[0]) && (m[1] == rhs.m[1]) && (m[2] == rhs.m[2]) &&
				(m[3] == rhs.m[3]) && (m[4] == rhs.m[4]) && (m[5] == rhs.m[5]) &&
				(m[6] == rhs.m[6]) && (m[7] == rhs.m[7]) && (m[8] == rhs.m[8]);
		}        

		bool operator!=(const Matrix3 rhs) const {
			return !(*this == rhs);
		}

		double& operator[](size_t idx)
		{
			assert (idx <= 8);
			return m[idx];
		}

		const double& operator[](size_t idx) const
		{
			assert (idx <= 8);
			return m[idx];
		}        

    private:
        double m[9];
    };
}

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
