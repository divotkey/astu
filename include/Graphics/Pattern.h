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
         * Rotates this patten by the specified angle.
         *
         * @param phi   the angle in degrees
         */
        void Rotate(double phi);

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
    };

} // end of namespace