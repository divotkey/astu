/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "math/Vector2.h"
#include "math/Matrix3.h"

namespace astu {


    /**
     * An axis alinged bounding box.
     */
    class BoundingBox final {
    public:

        static BoundingBox CreateInfinite();

        /**
         * Constructor.
         */
        BoundingBox();

        /**
         * Constructor.
         * 
         * @param w the width of this bounding box
         * @param h the height of this bounding box
         */
        BoundingBox(double w, double h);

        /**
         * Sets the center of this box to (0, 0) and its size to zero.
         */
        void Reset();

        void SetWidth(double w);
        void SetHeight(double h);

        double GetWidth() const {
            return width;
        }

        double GetHeight() const {
            return height;
        }

        double GetHRadius() const {
            return hRadius;
        }

        double GetVRadius() const {
            return vRadius;
        }

        void SetCenter(const Vector2 & c);

        void MoveCenter(const Vector2 & tx);
        void MoveCenter(double dx, double dy);

        const Vector2 & GetCenter() const {
            return center;
        }

        double GetLeftBound() const {
            return center.x - hRadius;
        }

        double GetRightBound() const {
            return center.x + hRadius;
        }

        double GetUpperBound() const {
            return center.y + vRadius;
        }

        double GetLowerBound() const {
            return center.y - vRadius;
        }

        Vector2 GetUpperLeft() const {
            return Vector2(GetLeftBound(), GetUpperBound());
        }

        Vector2 GetUpperRight() const {
            return Vector2(GetRightBound(), GetUpperBound());
        }

        Vector2 GetLowerLeft() const {
            return Vector2(GetLeftBound(), GetLowerBound());
        }

        Vector2 GetLowerRight() const {
            return Vector2(GetRightBound(), GetLowerBound());
        }

        bool IsInfinite() const {
            return infinite;
        }

        bool IsZero() const {
            return !IsInfinite() && (width == 0) && (height == 0);
        }

        bool IsInside(const Vector2 & p) const;
        bool IsInside(const BoundingBox & box) const;

        void AddPoint(const Vector2 & p);
        void Merge(const BoundingBox & o);

        void Transform(const Matrix3 & tx);


    private:
        /** The center of this bounding box. */
        Vector2 center;

        /** The width of thsi bounding box. */
        double width;

        /** The height of thsi bounding box. */
        double height;

        /** Horicontal radius of this bounding box (height / 2). */
        double hRadius;

        /** Vertical radius of this bounding box (width / 2). */
        double vRadius;

        /** Determines that this bounding box is of infinite size. */
        bool infinite;
    };

}