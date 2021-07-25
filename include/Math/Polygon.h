/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes.
#include "Vector2.h"
#include "Segment1.h"
#include "Segment2.h"
#include "Transform2.h"
#include "Graphics/VertexBuffer2.h"

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>
#include <vector>

namespace astu {

    /**
     * Mathematical utility class representing a polygon.
     * 
     * In geometry, a polygon is a plane figure that is described by a finite
     * number of straight line segments connected to form a closed polygonal
     * chain (or polygonal circuit). 
     * 
     * @tparam T    the numerical type used to represent this polygon
     * @ingroup math_group
     */
    template <typename T>
    class Polygon : public VertexBuffer2<T> {
    public:

        /**
         * Constructor.
         * 
         * @param vertices  a vector containing the vertices of this polygon
         * @throws  std::logic_error in case the number of
         *          vertices is less than three.
         */
        Polygon(const std::vector<Vector2<T>>& vertices)
            : vertices(vertices)
        {
            if (vertices.size() < 3) {
                throw std::logic_error(
                    "Invalid number of vertices to define a polygon");
            }            
        }

        /**
         * Returns the vertices of this polygon.
         * 
         * @return a vector containing the vertices
         */
        const std::vector<Vector2<T>>& GetVertices() const {
            return vertices;
        }

        /**
         * Returns the number of vertices of this polygon.
         * 
         * @return the number of vertices
         */
        size_t NumVertices() const {
            return vertices.size();
        }

        /**
         * Returns the number of edges of this polygon.
         * 
         * @return the number of edges
         */
        size_t NumEdges() const {
            return vertices.size();
        }

        /**
         * Returns the vertex with the specified index.
         * 
         * @param idx   the vertex index
         * @return the vertex index
         */
        const Vector2<T>& GetVertex(size_t idx) const {
            assert(idx < vertices.size());
            return vertices[idx];
        }

        /**
         * Returns the with the specified index as two-dimensional segment.
         * 
         * @param idx   the edge index
         * @return the segment representing the edge
         */
        Segment2<T> GetEdgeAsSegment(size_t idx) const {
            assert(idx < vertices.size() );
            return Segment2<T>(vertices[idx], vertices[(idx + 1) % vertices.size()]);
        }

        /**
         * Returns the edge with the specified index.
         * 
         * An edge is represented as the direction vector pointing from
         * the start vertex of the edge to the end vertex.
         * 
         * @param idx   the edge index
         * @return the edge vector
         */
        Vector2<T> GetEdge(size_t idx) const {
            assert(idx < vertices.size() );
            return vertices[(idx + 1) % vertices.size()] - vertices[idx];   
        }

        /**
         * Returns the center point of the edit with the specified index.
         * 
         * @param idx   the index of the edge
         * @return the center point of the edge
         */
        Vector2<T> GetEdgeCenter(size_t idx) const {
            return GetVertex(idx) + GetEdge(idx) / 2;
        }

        /**
         * Returns the normal vector of the edge with the specified index.
         * 
         * @param idx   the edge index
         * @return the normal vector of the edge
         */
        Vector2<T> GetEdgeNormal(size_t idx) const {
            assert(idx < vertices.size() );
            return GetEdge(idx).Perpendicularize().Normalize();
        }

        /**
         * Tests whether a point lies within this polygon.
         * 
         * One simple way of finding whether the point is inside or outside a
         * simple polygon is to test how many times a ray, starting from the
         * point and going in any fixed direction, intersects the edges of the
         * polygon. If the point is on the outside of the polygon the ray will
         * intersect its edge an even number of times. If the point is on the
         * inside of the polygon then it will intersect the edge an odd number
         * of times. https://en.wikipedia.org/wiki/Point_in_polygon
         * 
         * @param p the point to test
         * @return `true` if the point lies within the polygon
         */
        bool IsInside(const Vector2<T>& p) const {
            // Crearte a ray pointing arbitrary ray direction.
            Ray2<T> ray(p.x, p.y, static_cast<T>(1.2), static_cast<T>(3.4));

            int cnt = 0;
            for (size_t i = 0; i < NumEdges(); ++i) {
                if (GetEdgeAsSegment(i).IsIntersecting(ray)) {
                    ++cnt;
                }
            }

            return cnt % 2 != 0;
        }

        /**
         * Projects this polygon on the specified axis.
         * 
         * @param axis  the axis used to project this polygon
         * @return the 1D segment representing the projected polygon
         */
        Segment1<T> Project(const Vector2<T>& axis) const {
            Segment1<T> result;

            auto it = vertices.begin();
            result.Set( axis.Dot(*it++) );
            for (; it != vertices.end(); ++it) {
                result.Extend( axis.Dot(*it) );
            }

            return result;
        }

        /**
         * Transforms this polygon by the specified transformation.
         * 
         * @param tx    the transfromation
         * @return reference to this polygon for method chaining
         */
        Polygon& Transform(const Transform2<T>& tx) {
            for (auto & vertex : vertices) {
                tx.TransformPointIp(vertex);
            }
            return *this;
        }

    private:
        /** The vertices of this polygon. */
        std::vector<Vector2<T>> vertices;
    };

    /**
     * Type alias for Polygon template using double as data type.
     * 
     * Polygon are ony defined in two-dimensional space, hence the `2` in
     * this alias is redundant. However, it fits to other naming conventions
     * for classes that do exist in other dimensions on the one hand and 
     * on the other hand it helps to separate the `d` postfix from the 
     * class name.
     */
    using Polygon2d = astu::Polygon<double>;    

    /**
     * Type alias for Polygon template using float as data type.
     * 
     * Polygon are ony defined in two-dimensional space, hence the `2` in
     * this alias is redundant. However, it fits to other naming conventions
     * for classes that do exist in other dimensions on the one hand and 
     * on the other hand it helps to separate the `f` postfix from the 
     * class name.
     */
    using Polygon2f = astu::Polygon<float>;    


} // end of namespace