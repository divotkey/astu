/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Pattern.h"
#include "Graphics/QuadtreePattern.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    // Forward declaration
    class TrianglePattern;

    /**
     * A pattern representing a polygonal shape with n vertices.
     */
    class PolygonPattern2 : public Pattern {
    public:

        /**
         * Constructor.
         */
        PolygonPattern2();

        /**
         * Adds a vertex to this polygon pattern.
         *
         * @param vtx   the vertex to add
         */
        void AddVertex(const Vector2d &vtx);

        /**
         * Adds a vertex to this polygon pattern.
         *
         * @param vx    the x-coordinate of the vertex to add
         * @param vy    the y-coordinate of the vertex to add
         */
        void AddVertex(double vx, double vy) {
            AddVertex(Vector2d(vx, vy));
        }

        /**
         * Retruns the number of vertices of this polygon.
         *
         * @return the number of vertices
         */
        size_t NumVertices() const {
            return vertices.size();
        }

        /**
         * Sets the pattern of this polygon.
         *
         * @param p the pattern
         */
        void SetPattern(std::shared_ptr<Pattern> p) {
            pattern = p;
        }

        /**
         * Returns the pattern of this polygon.
         *
         * @return  the pattern
         */
        std::shared_ptr<Pattern> GetPattern() {
            return pattern;
        }

        /**
         * Sets the pattern of the outline this polygon.
         *
         * @param p the pattern
         */
        void SetOutlinePattern(std::shared_ptr<Pattern> p) {
            outlinePattern = p;
        }

        /**
         * Returns the pattern of this polygon.
         *
         * @return  the pattern
         */
        std::shared_ptr<Pattern> GetOutlinePattern() {
            return outlinePattern;
        }

        /**
         * Returns whether a pattern has been specified by this polygon.
         *
         * @return `true` if a pattern has been specified
         */
        bool HasOutlinePattern() const {
            return outlinePattern != nullptr;
        }

        /**
         * Sets the width of the outline of this polygon.
         *
         * @param w the outline width
         */
        void SetOutlineWidth(double w) {
            outlineWidth = w;
        }

        /**
         * Returns the width of the outline of this polygon.
         *
         * @return the with of the outline
         */
        double GetOutlineWidth() const {
            return outlineWidth;
        }

        /**
         * Returns whether a pattern has been specified by this polygon.
         *
         * @return `true` if a pattern has been specified
         */
        bool HasPattern() const {
            return pattern != nullptr;
        }

        /**
         * Calculates the geometric center (centroid) of this polygon.
         *
         * @return vector resembling the geometric center of all vertices of this polygon
         */
        Vector2d CalcGeometricCenter() const;

        void SetDebugQuadtree(bool b);

        bool IsDebugQuadtree() const;

        /**
         * Enables or disables debug visualization.
         *
         * @param b
         */
        void SetDebug(bool b);

        /**
         * Returns whether debug visualization is enabled or disabled.
         *
         * @return `true` if debug visualization is enabled
         */
        bool IsDebug() const;

        /**
         * Specifies the maximum number of elements per node.
         *
         * @param n the maximum number of elements
         */
        void SetMaxElems(int n) {
            quadtree->SetMaxElems(n);
        }

        /**
         * Returns the maximum number of elements per node.
         *
         * @return the maximum number of elements
         */
        int GetMaxElems() const {
            return quadtree->GetMaxElems();
        }

        /**
         * Specifies the maximum recursion depth of this quadtree.
         *
         * @param n the maximum recursion depth
         */
        void SetMaxDepth(int n) {
            quadtree->SetMaxDepth(n);
        }

        /**
         * Returns the maximum recursion depth of this quadtree.
         *
         * @return the maximum recursion depth
         */
        int GetMaxDepth() const {
            return quadtree->GetMaxDepth();
        }

        void SetScan(bool b) {
            quadtree->SetScan(b);
        }

        void SetScanFactor(double p) {
            quadtree->SetScanFactor(p);
        }

        // Inherited via pattern
        void Prepare() override;

    protected:
        // Inherited via pattern
        bool GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const override;
        BoundingBox GetLocalBoundingBox() const override;
        bool IsInsideTransformed(const Vector2<double> &pt) const override;

    private:
        /** The vertices of this polygon. */
        std::vector<Vector2d> vertices;

        /** The pattern used to define the color of polygon. */
        std::shared_ptr<Pattern> pattern;

        /** The pattern of the outline of this polygon. */
        std::shared_ptr<Pattern> outlinePattern;

        /** The width of the outline of this polygon. */
        double outlineWidth;

        /** Used to organize and store the triangles of this polygon. */
        std::shared_ptr<QuadtreePattern> quadtree;

        /** The actual pattern used to render this polygon. */
        std::shared_ptr<Pattern> delegatePattern;




        /** Whether to visualize triangulation. */
        bool debug;

        bool IsEar(const std::vector<Vector2d> &lst, const Vector2d &p1, const Vector2d &p, const Vector2d &p2) const;
        bool IsInsideTriangle(const Vector2d &p0, const Vector2d &p1, const Vector2d &p2, const Vector2d &p) const;
        bool IsClockwise() const;
        size_t ChooseConvexHullVertex() const;
        std::shared_ptr<TrianglePattern> CreateTriangle(const Vector2d &p1, const Vector2d &p2, const Vector2d &p3);
        std::shared_ptr<Pattern> CreateOutline();
    };

} // end of namespace