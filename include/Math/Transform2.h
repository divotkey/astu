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
#include "Math/Vector2.h"
#include "Math/Matrix3.h"

namespace astu {

    /**
     * A affine transformation in two-dimensional space.
     * 
     * @ingroup math_group
     */
    template <typename T>
    class Transform2 {
    public:

        /**
         * Constructor.
         */
        Transform2()
            : translation(0, 0), scaling(1, 1), rotation(0), dirty(false)
        {
            // Intentionally left empty
        }

        /**
         * Sets this transform to its identity.
         * 
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetIdentity() {
            SetTranslation(0, 0);
            SetScaling(1, 1);
            SetRotation(0);
            dirty = true;
            return *this;
        }

        /**
         * Sets the translation of this transform.
         * 
         * @param tx    the translation on the x-dimension
         * @param ty    the translation on the y-dimension
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetTranslation(T tx, T ty) {
            translation.Set(tx, ty);
            dirty = true;
            return *this;
        }

        /**
         * Sets the translation of this transform on the x-axis.
         * 
         * @param tx    the translation on the x-dimension
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetTranslationX(T tx) {
            translation.x = tx;
            dirty = true;
            return *this;
        }

        /**
         * Sets the translation of this transform on the y-axis.
         * 
         * @param ty    the translation on the y-dimension
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetTranslationY(T ty) {
            translation.y = ty;
            dirty = true;
            return *this;
        }

        /**
         * Sets the translation of this transform.
         * 
         * @param tv    the translation vector
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetTranslation(const Vector2<T> & tv) {
            translation = tv;
            dirty = true;
            return *this;
        }

        /**
         * Adds a translation to this transform.
         * 
         * @param tx    the delta movement on the x-axis
         * @param ty    the delta movement on the y-axis
         * @return reference of this transform for method chaining
         */
        Transform2<T> & Translate(T tx, T ty) {
            translation.Add(tx, ty);
            dirty = true;
            return *this;
        }

        /**
         * Adds a translation to this transform.
         * 
         * @param tv    a vector defining the delta movement
         * @return reference of this transform for method chaining
         */
        Transform2<T> & Translate(const Vector2<T> & tv) {
            translation += tv;
            dirty = true;
            return *this;
        }

        /**
         * Returns the translation of this transform.
         * 
         * @return the translation vector
         */
        const Vector2<T> & GetTranslation() const {
            return translation;
        }

        /**
         * This method sets the scaling of this transformation.
         * 
         * @param sx    the scaling factor on the x-axis
         * @param sy    the scaling factor on the y-axis
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetScaling(T sx, T sy) {
            scaling.Set(sx, sy);
            dirty = true;
            return *this;
        }

        /**
         * This method sets the scaling of this transformation.
         * 
         * @param sv    the scaling vector
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetScaling(const Vector2<T> & sv) {
            scaling = sv;
            dirty = true;
            return *this;
        }

        /**
         * Scales this transform.
         * 
         * @param sx    the additional scaling factor for the x-axis
         * @param sy    the additional scaling factor for the y-axis
         * @return reference of this transform for method chaining
         */
        Transform2<T> & Scale(T sx, T sy) {
            scaling.Scale(sx, sy);
            dirty = true;
            return *this;
        }

        /**
         * Scales this transform.
         * 
         * @param sv    the additional scaling vector
         * @return reference of this transform for method chaining
         */
        Transform2<T> & Scale(const Vector2<T> & sv) {
            scaling *= sv;
            dirty = true;
            return *this;
        }

        /**
         * Returns the scaling of this transform.
         * 
         * @return the scaling vector
         * @return reference of this transform for method chaining
         */
        const Vector2<T> & GetScaling() const {
            return scaling;
        }

        /**
         * Sets the rotation of this transform.
         * 
         * @param phi   the rotation angle in radians
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetRotation(T phi) {
            rotation = phi;
            dirty = true;
            return *this;
        }

        /**
         * Sets the rotation of this transform.
         * 
         * @param phi   the rotation angle in degrees
         * @return reference of this transform for method chaining
         */
        Transform2<T> & SetRotationDeg(T phi) {
            rotation = MathUtils::ToRadians<T>(phi);
            dirty = true;
            return *this;
        }

        /**
         * Rotates this transform.
         * 
         * @param deltaPhi  the delta angle in degrees
         * @return reference of this transform for method chaining
         */
        Transform2<T> & RotateDeg(T deltaPhi) {
            rotation += MathUtils::ToRadians<T>(deltaPhi);
            dirty = true;
            return *this;
        }

        /**
         * Rotates this transform.
         * 
         * @param deltaPhi  the delta angle in degrees
         * @return reference of this transform for method chaining
         */
        Transform2<T> & Rotate(T deltaPhi) {
            rotation += deltaPhi;
            dirty = true;
            return *this;
        }

        /**
         * Returns the rotation of this transform.
         * 
         * @return the rotation angle in radians
         */
        T GetRotation() const {
            return rotation;
        }

        /**
         * Transform a point.
         * 
         * @param inPoint   the point to transform
         * @return the transformed point
         */
        Vector2<T> TransformPoint(const Vector2<T> & inPoint) const {
            return Vector2<T>(inPoint)
                .Scale(scaling)
                .Rotate(rotation)
                .Add(translation);
        }

        /**
         * Transform a point.
         * 
         * @param x the x-coordinate of the point to transform
         * @param y the y-coordinate of the point to transform
         * @return the transformed point
         */
        Vector2<T> TransformPoint(T x, T y) const {
            return Vector2<T>(x, y)
                .Scale(scaling)
                .Rotate(rotation)
                .Add(translation);
        }

        /**
         * Transforms a point in-place.
         * 
         * @param inOutPoint    the point to transform
         * @return a reference of the specified and now transformed point
         */
        Vector2<T>& TransformPointIp(Vector2<T> & inOutPoint) const {
            return inOutPoint.Scale(scaling).Rotate(rotation).Add(translation);
        }

        /**
         * Transform a vector.
         * 
         * @param inVector  the vector transform
         * @return the transformed vector
         */
        Vector2<T> TransformVector(const Vector2<T> & inVector) const {
            return Vector2<T>(inVector)
                .Scale(scaling)
                .Rotate(rotation);
        }

        /**
         * Transform a vector.
         * 
         * @param vx    the x-coordinate of the vector to transform
         * @param vy    the y-coordinate of the vector to transform
         * @return the transformed vector
         */
        Vector2<T> TransformVector(T vx, T vy) const {
            return Vector2<T>(vx, vy)
                .Scale(x, y)
                .Rotate(rotation);
        }

        /**
         * Transforms a vector in-place.
         * 
         * @param inOutVector   the vector to transform
         * @return a reference of the specified and now transformed vector
         */
        Vector2<T>& TransformVectorIp(Vector2<T> & inOutVector) const {
            return inOutVector.Scale(scaling).Rotate(rotation);
        }
        
        /**
         * Stores the transformation in a matrix.
         * 
         * @param outM  receives the transformation matrix of this transform
         * @return reference to the specified transformation matrix
         */
        const Matrix3<T>& StoreToMatrix(Matrix3<T>& outM) const {
            outM.SetToScale(scaling);
            outM.Rotate(rotation);
            outM.Translate(translation);
            return m;
        }

        /**
         * Calculates the transformation matrix of this transform.
         * 
         * @return the transformation matrix of this transform
         */
        Matrix3<T> CalcMatrix() const {
            return StoreToMatrix(Matrix3<T>());
        }

        /**
         * Returns whether this transformation has been changed.
         * 
         * This method return whether this transformation has changed since
         * the last call to ClearDirty().
         * 
         * @return `true` if the transformation has changed
         */
        bool IsDirty() const {
            return dirty;
        }

        /**
         * Clears the dirty flag of this transformation.
         * 
         * @return reference of this transform for method chaining
         */
        Transform2<T> & ClearDirty() {
            dirty = false;
            return *this;
        }

    private:
        /** Translation of this transformation, as a vector. */
        astu::Vector2<T> translation;

        /** 2D scale (always applied in local space) as a vector. */
        astu::Vector2<T> scaling;

        /** Rotation of this transformation, in radian. */
        T rotation;

        /** Indicates whether this transform is dirty. */
        bool dirty;
    };

    /**
     * Type alias for Transform template using double as data type.
     * 
     * @ingroup math_group
     */
    using Transform2d = Transform2<double>;

    /**
     * Type alias for Transform template using float as data type.
     * 
     * @ingroup math_group
     */
    using Transform2f = Transform2<float>;

} // end of namespace