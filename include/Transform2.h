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
#include "Vector2.h"
#include "Matrix3.h"

namespace astu {

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

        Transform2<T> & SetIdentity() {
            SetTranslation(0, 0);
            SetScaling(1, 1);
            SetRotation(0);
            dirty = true;
            return *this;
        }

        Transform2<T> & SetTranslation(T tx, T ty) {
            translation.Set(tx, ty);
            dirty = true;
            return *this;
        }

        Transform2<T> & SetTranslationX(T tx) {
            translation.x = tx;
            dirty = true;
            return *this;
        }

        Transform2<T> & SetTranslationY(T ty) {
            translation.y = ty;
            dirty = true;
            return *this;
        }

        Transform2<T> & SetTranslation(const Vector2<T> & t) {
            translation = t;
            dirty = true;
            return *this;
        }

        Transform2<T> & Translate(T tx, T ty) {
            translation.Add(tx, ty);
            dirty = true;
            return *this;
        }

        Transform2<T> & Translate(const Vector2<T> & t) {
            translation += t;
            dirty = true;
            return *this;
        }

        const Vector2<T> & GetTranslation() const {
            return translation;
        }

        Transform2<T> & SetScaling(T sx, T sy) {
            scaling.Set(sx, sy);
            dirty = true;
            return *this;
        }

        Transform2<T> & SetScaling(const Vector2<T> & s) {
            scaling = s;
            dirty = true;
            return *this;
        }

        Transform2<T> & Scale(T sx, T sy) {
            scaling.Scale(sx, sy);
            dirty = true;
            return *this;
        }

        Transform2<T> & Scale(const Vector2<T> & s) {
            scaling *= s;
            dirty = true;
            return *this;
        }

        const Vector2<T> & GetScaling() const {
            return scaling;
        }

        Transform2<T> & SetRotation(T phi) {
            rotation = phi;
            dirty = true;
            return *this;
        }

        Transform2<T> & SetRotationDeg(T phi) {
            rotation = MathUtils::ToRadians<T>(phi);
            dirty = true;
            return *this;
        }

        Transform2<T> & RotateDeg(T deltaPhi) {
            rotation += MathUtils::ToRadians<T>(deltaPhi);
            dirty = true;
            return *this;
        }

        Transform2<T> & Rotate(T deltaPhi) {
            rotation += deltaPhi;
            dirty = true;
            return *this;
        }

        T GetRotation() const {
            return rotation;
        }

        Vector2<T> TransformPoint(const Vector2<T> & p) const {
            return Vector2<T>(p)
                .Scale(scaling)
                .Rotate(rotation)
                .Add(translation);
        }

        Vector2<T> TransformPoint(T x, T y) const {
            return Vector2<T>(x, y)
                .Scale(scaling)
                .Rotate(rotation)
                .Add(translation);
        }

        Vector2<T> TransformVector(const Vector2<T> & v) const {
            return Vector2<T>(v)
                .Scale(scaling)
                .Rotate(rotation);
        }

        Vector2<T> TransformVector(T x, T y) const {
            return Vector2<T>(x, y)
                .Scale(x, y)
                .Rotate(rotation);
        }

        const Matrix3<T>& StoreToMatrix(Matrix3<T>& m) const {
            m.SetToScale(scaling);
            m.Rotate(rotation);
            m.Translate(translation);
            return m;
        }

        Matrix3<T> CalcMatrix() const {
            return StoreToMatrix(Matrix3<T>());
        }

        bool IsDirty() const {
            return dirty;
        }

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

        /** Indicats whether this transform is dirty. */
        bool dirty;
    };

    using Transform2f = Transform2<float>;
    using Transform2d = Transform2<double>;

} // end of namespace