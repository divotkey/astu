/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Pattern.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    /**
     * A pattern representing a polygonal shape with n vertices.
     *
     * 1.) Draw a horizontal line to the right of each point and extend it to infinity.
     * 2.) Count the number of times the line intersects with polygon edges.
     * 3.) A point is inside the polygon if either count of intersections is odd or point lies on an edge of polygon.
     * If none of the conditions is true, then point lies outside.
     *
     * http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
     */
    class PolygonPattern : public Pattern {
    public:

        /**
         * Constructor.
         */
        PolygonPattern();

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

        /**
         * Calculates the distance from a point to the nearest edge of this polygon.
         *
         * @param p the point to test
         * @return the distance to the nearest edge
         */
        double CalcDistance(const Vector2d &p) const;

    protected:
        // Inherited via pattern
        virtual bool GetColorTransformed(const Vector2<double> &pt, Color4d &outColor) const override;
        virtual BoundingBox GetLocalBoundingBox() const override;

        bool IsInsideTransformed(const Vector2<double> &pt) const override;

    private:
        /** The vertices of this polygon. */
        std::vector<Vector2d> vertices;

        /** The pattern of this polygon. */
        std::shared_ptr<Pattern> pattern;

        /** The pattern of the outline of this polygon. */
        std::shared_ptr<Pattern> outlinePattern;

        /** The width of the outline of this polygon. */
        double outlineWidth;
    };

} // end of namespace