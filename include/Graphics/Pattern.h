/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Color.h"
#include "Graphics/BoundingBox.h"
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
#include "Velox/Interpreter/ItemData.h"

namespace astu {

    /**
     * Base class for all Patterns.
     *
     * Note: deriving this base class from ItemData makes integration into Velox scripting language
     * way more easier.
     */
    class Pattern : public velox::ItemData {
    public:

        /**
         * Constructor.
         */
        Pattern();

        /**
         * Virtual destructor.
         */
        virtual ~Pattern() {}

        /**
         * Translates this pattern by the specified translation vector.
         *
         * @param vt    the translation vector
         */
        void Translate(const Vector2d & vt) {
            Translate(vt.x, vt.y);
        }

        /**
         * Translates this pattern by the specified translation vector.
         *
         * @param tx    the x-component of the translation vector
         * @param ty    the y-component of the translation vector
         */
        void Translate(double tx, double ty);

        /**
         * Translates this pattern by the specified translation vector.
         * This method transforms in geometric logical order instead of mathematical order.
         *
         * @param tx    the x-component of the translation vector
         * @param sy    the y-component of the translation vector
         */
        void TranslateGeometric(double tx, double ty);

        /**
         * Translates this pattern by the specified translation vector.
         * This method transforms in geometric logical order instead of mathematical order.
         *
         * @param vt    the translation vector
         */
        void TranslateGeometric(const Vector2d & vt) {
            TranslateGeometric(vt.x, vt.y);
        }

        /**
         * Scales this pattern by the specified scaling vector.
         *
         * @param vt    the scaling vector
         */
        void Scale(const Vector2d & vs) {
            Scale(vs.x, vs.y);
        }

        /**
         * Scales this pattern by the specified scaling vector.
         *
         * @param sx    the x-component of the scaling vector
         * @param sy    the y-component of the scaling vector
         */
        void Scale(double sx, double sy);

        /**
         * Scales this pattern by the specified scaling vector.
         * This method transforms in geometric logical order instead of mathematical order.
         *
         * @param sx    the x-component of the scaling vector
         * @param sy    the y-component of the scaling vector
         */
        void ScaleGeometric(double sx, double sy);

        /**
         * Scales this pattern by the specified scaling vector in geometric order.
         * This method transforms in geometric logical order instead of mathematical order.
         *
         * @param vt    the scaling vector
         */
        void ScaleGeometric(const Vector2d & vs) {
            ScaleGeometric(vs.x, vs.y);
        }

        /**
         * Rotates this patten by the specified angle.
         *
         * @param phi   the angle in radians
         */
        void Rotate(double phi);

        /**
         * Rotates this patten by the specified angle.
         * This method transforms in geometric logical order instead of mathematical order.
         *
         * @param phi   the angle in radians
         */
        void RotateGeometric(double phi);

        /**
         * Clears all previously applied transformations.
         */
        void ClearTransform();

        /**
         * Returns the bounding box of this pattern.
         *
         * @return the bounding box
         */
        BoundingBox GetBoundingBox() const;

        /**
         * Determines the color of this patter at the specified point.
         *
         * @param p         the point in question
         * @param outColor  receives the determined color
         * @return `true` if the specified point lies within this pattern, `false` otherwise
         */
        virtual bool GetColor(const Vector2<double> &p, Color4d &outColor) const;

        /**
         * Tests whether a point is inside or outside this pattern.
         *
         * @param p the point to be tested
         * @return `true` if the point is in side this pattern
         */
         virtual bool IsInside(const Vector2<double> &p) const;

         /**
          * Tests whether this pattern either intersects or lies entirely inside a bounding box.
          *
          * @param bbox the bounding box to test
          * @return `true` if this pattern intersects or lies entirely in side the bounding box
          */
         //virtual bool IsInside(const BoundingBox &bbox) const;

        /**
         * Prepares this patter for rendering.
         */
        virtual void Prepare();

    protected:

        /**
         * Determines the color of this patter at the specified local point.
         *
         * @param pt        the point in question in local space
         * @param outColor  receives the determined color
         * @return `true` if the specified point lies within this pattern, `false` otherwise
         */
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & outColor) const = 0;

        /**
         * Tests whether a point is inside or outside this pattern.
         *
         * @param pt    the point in question in local space
         * @return `true` if the point is in side this pattern
         */
        virtual bool IsInsideTransformed(const Vector2<double> &pt) const = 0;


        /**
         * Returns the bounding box of this pattern in local space.
         *
         * @return  the bounding box in local space
         */
        virtual BoundingBox GetLocalBoundingBox() const = 0;

    private:
        /** The transformation matrix of this pattern. */
        Matrix3d transform;

        /** Indicates whether the bounding box is dirty. */
        mutable bool dirty;

        /** The cached bounding box of this pattern. */
        mutable BoundingBox boundingBox;

        /**
         * Recalculates the bounding box based on current transformation.
         */
        void UpdateBoundingBox() const;
    };

} // end of namespace