/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
#include "Graphics/Color.h"
#include "Graphics/WebColors.h"
#include "Velox/Interpreter/ItemData.h"
#include "BoundingBox.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    /////////////////////////////////////////////////
    /////// Class Patterns
    /////////////////////////////////////////////////

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


    /////////////////////////////////////////////////
    /////// Class UnicolorPattern
    /////////////////////////////////////////////////

    /**
     * An infinite pattern representing one single color.
     */
    class UnicolorPattern : public Pattern {
    public:

        /**
         * Constructor.
         *
         * @param c the one and only color of this pattern
         */
        UnicolorPattern(const Color4d &c = WebColors::Cyan) : color(c) {}

        /**
         * Returns the one and only color of this pattern.
         *
         * @return the pattern's color
         */
        const Color4d &GetColor() const {
            return color;
        }

        /**
         * Sets the one and only color of this pattern.
         *
         * @param c the one and only color of this pattern
         */
        void SetColor(const Color4d &c) {
            color = c;
        }

        // Inherited via Pattern
        virtual bool GetColor(const Vector2<double> &p, Color4d &outColor) const override;

    protected:

        // Inherited via Pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        /** The one and only color of this pattern. */
        Color4d color;
    };


    /////////////////////////////////////////////////
    /////// Class RectanglePattern
    /////////////////////////////////////////////////

    /**
     * A pattern representing a rectangular shape.
     */
    class RectanglePattern : public Pattern {
    public:

        /**
         * Constructs an axis aligned rectangular shape
         *
         * @param w the width of this rectangle
         * @param h the height of this rectangle
         */
        RectanglePattern(double w = 1, double h = 1);

        /**
         * Sets the width of this rectangle.
         *
         * @param w the width
         * @throws std::domain_error in case the width is less or equal zero
         */
        void SetWidth(double w);

        /**
         * Sets the height of this rectangle.
         *
         * @param w the height
         * @throws std::domain_error in case the height is less or equal zero
         */
        void SetHeight(double h);

        /**
         * Sets the dimensions of this rectangle.
         *
         * @param w the width of this rectangle
         * @param h the height of this rectangle
         * @throws std::domain_error in case the width or height height is less or equal zero
         */
        void SetSize(double w, double h) {
            SetWidth(w);
            SetHeight(h);
        }

        /**
         * Returns the width of this rectangle.
         *
         * @return the width
         */
        double GetWidth() const {
            return width;
        }

        /**
         * Returns the height of this rectangle.
         *
         * @return the height
         */
        double GetHeight() const {
            return height;
        }

        /**
         * Sets the pattern of this rectangle.
         *
         * @param p the pattern
         */
        void SetPattern(std::shared_ptr<Pattern> p) {
            pattern = p;
        }

        /**
         * Returns the pattern of this rectangle.
         *
         * @return  the pattern
         */
        std::shared_ptr<Pattern> GetPattern() {
            return pattern;
        }

        /**
         * Returns whether a pattern has been specified by this rectangle.
         *
         * @return `true` if a pattern has been specified
         */
        bool hasPattern() const {
            return pattern != nullptr;
        }

    protected:

        // Inherited via pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        /** The width of this rectangle. */
        double width;

        /** The height of this rectangle. */
        double height;

        /** The horizontal radius (width / 2) of this rectangle. */
        double hRadius;

        /** The vertical radius (height / 2) of this rectangle. */
        double vRadius;

        /** The pattern of this rectangle. */
        std::shared_ptr<Pattern> pattern;
    };


    /////////////////////////////////////////////////
    /////// Class CirclePattern
    /////////////////////////////////////////////////

    /**
     * A pattern representing a circular shape.
     */
    class CirclePattern : public Pattern {
    public:

        /**
         * Constructor.
         *
         * @param r the radius of this circular shape
         */
        CirclePattern(double r = 1);

        /**
         * Sets the radius of this circular shape.
         *
         * @param r the radius
         * @throws std::domain_error in case the radius is less or equal zero
         */
        void SetRadius(double r);

        /**
         * Returns the radius of this circular shape.
         *
         * @return the radius
         */
        double getRadius() const {
            return radius;
        }


        /**
         * Sets the pattern of this rectangle.
         *
         * @param p the pattern
         */
        void SetPattern(std::shared_ptr<Pattern> p) {
            pattern = p;
        }

        /**
         * Returns the pattern of this rectangle.
         *
         * @return  the pattern
         */
        std::shared_ptr<Pattern> GetPattern() {
            return pattern;
        }

        /**
         * Returns whether a pattern has been specified by this rectangle.
         *
         * @return `true` if a pattern has been specified
         */
        bool hasPattern() const {
            return pattern != nullptr;
        }

    protected:
        // Inherited via pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        /** The radius of this circular shape. */
        double radius;

        /** The radius of this circular shape squared. */
        double radiusSquared;

        /** The pattern of this rectangle. */
        std::shared_ptr<Pattern> pattern;
    };

    class CompoundPattern : public Pattern {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~CompoundPattern() {}

        /**
         * Adds the specified pattern to this compound.
         */
        void Add(std::shared_ptr<Pattern> child);

        /**
         * Removes all child patterns from this compound.
         */
        void Clear();

        bool IsEmpty() const {
            return children.empty();
        }

    	std::vector<std::shared_ptr<Pattern>>::iterator begin() {
            return children.begin();
        }

        std::vector<std::shared_ptr<Pattern>>::iterator end() {
            return children.end();
        }

    protected:
        std::vector<std::shared_ptr<Pattern>> children;

        virtual void OnPatternAdded(Pattern & pattern) {}
        virtual void OnClear() {}
    };

    class UnionPattern : public CompoundPattern {
    public:

    protected:
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        void Blend(Color4d & a, const Color4d & b) const;
    };

}