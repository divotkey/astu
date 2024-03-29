/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
#include "Graphics/Color.h"
#include "BoundingBox.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    class Pattern {
    public:

        Pattern();
        virtual ~Pattern() {}

        void Translate(const Vector2<double> & t) {
            Translate(t.x, t.y);
        }

        void Translate(double x, double y);
        void Scale(double sx, double sy);
        void Rotate(double phi);

        BoundingBox GetBoundingBox() const;
        virtual bool GetColor(const Vector2<double> &p, Color4d & c) const;

    protected:
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const = 0;
        virtual BoundingBox GetLocalBoundingBox() const = 0;

    private:
        mutable bool dirty;
        mutable BoundingBox boundingBox;
        Matrix3d transform;
    };

    class UnicolorPattern : public Pattern {
    public:
    
        UnicolorPattern(const Color4d & c = Color4d(0, 1, 1))
            : color(c) {}
            
        const Color4d & GetColor() const {
            return color;
        }

        void SetColor(const Color4d & c) {
            color = c;
        }

        virtual bool GetColor(const Vector2<double> &p, Color4d & c) const override {
            c = color;
            return true;
        }


    protected:

        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override {
            // Should never get called.
            c = color;
            return true;
        }

        virtual BoundingBox GetLocalBoundingBox() const override {
            return BoundingBox::CreateInfinite();
        }

    private:
        Color4d color;
    };

    class RectanglePattern : public Pattern {
    public:

        RectanglePattern(double w = 1, double h = 1);

        void SetWidth(double w);
        void SetHeight(double h);

        void SetSize(double w, double h) {
            SetWidth(w);
            SetHeight(h);
        }

        double GetWidth() const {
            return width;
        }

        double GetHeight() const {
            return height;
        }

        void SetPattern(std::shared_ptr<Pattern> p) {
            pattern = p;
        }

        std::shared_ptr<Pattern> GetPattern() {
            return pattern;
        }

        bool hasPattern() const {
            return pattern != nullptr;
        }

    protected:
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        double width;
        double height;
        double hRadius;
        double vRadius;
        std::shared_ptr<Pattern> pattern;
    };

    class CirclePattern : public Pattern {
    public:

        CirclePattern(double r = 1);

        void SetRadius(double r);
        double getRadius() const {
            return radius;
        }

        void SetPattern(std::shared_ptr<Pattern> p) {
            pattern = p;
        }

        std::shared_ptr<Pattern> GetPattern() {
            return pattern;
        }

        bool hasPattern() const {
            return pattern != nullptr;
        }

    protected:
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d & c) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

    private:
        double radius;
        double radiusSquared;
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