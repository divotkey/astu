/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
#include "Velox/Interpreter/ItemData.h"

namespace astu {

    /**
     * An axis aligned bounding rectangle.
     * This class represents the two-dimensional version of a bounding box.
     */
    class BoundingBox final : public velox::ItemData {
    public:

        /**
         * Creates a bounding box with infinite dimensions.
         *
         * @return  the newly created bounding box
         */
        static BoundingBox CreateInfinite();

        /**
         * Constructor creating an empty bounding box.
         */
        BoundingBox();

        /**
         * Constructor.
         *
         * Creates a bounding rectangle width the specified dimensions with its center at the origin.
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

        void SetCenter(const Vector2<double> & c);

        void MoveCenter(const Vector2<double> & tx);
        void MoveCenter(double dx, double dy);

        const Vector2<double> & GetCenter() const {
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

        Vector2<double> GetUpperLeft() const {
            return Vector2<double>(GetLeftBound(), GetUpperBound());
        }

        Vector2<double> GetUpperRight() const {
            return Vector2<double>(GetRightBound(), GetUpperBound());
        }

        Vector2<double> GetLowerLeft() const {
            return Vector2<double>(GetLeftBound(), GetLowerBound());
        }

        Vector2<double> GetLowerRight() const {
            return Vector2<double>(GetRightBound(), GetLowerBound());
        }

        bool IsInfinite() const {
            return infinite;
        }

        /**
         * Tests whether this bounding box represents an empty bounding box with no dimensions and center.
         *
         * @return `true` if this is an empty bounding box
         */
        bool IsEmpty() const;

        bool IsInside(const Vector2<double> & p) const;
        bool IsInside(const BoundingBox & box) const;

        void AddPoint(const Vector2<double> & p);
        void Merge(const BoundingBox & o);

        void Transform(const Matrix3d & tx);


    private:
        /** The center of this bounding box. */
        Vector2<double> center;

        /** The width of this bounding box. */
        double width;

        /** The height of this bounding box. */
        double height;

        /** Horizontal radius of this bounding box (height / 2). */
        double hRadius;

        /** Vertical radius of this bounding box (width / 2). */
        double vRadius;

        /** Indicates that this bounding box is of infinite size. */
        bool infinite;
    };

} // end of namespace