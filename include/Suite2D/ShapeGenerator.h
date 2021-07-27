/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/VertexBuffer2.h"

// C++ Standard Library includes
#include <memory>

namespace astu::suite2d {

    /**
     * Utility class used to generate two-dimensional basic geometric shapes.
     * 
     * @ingroup suite2d_group
     */
    class ShapeGenerator final {
    public:

        /**
         * Constructor.
         */
        ShapeGenerator();

        /**
         * Specifies the vertex buffer builder to be used.
         * 
         * @param   builder the vertex buffer builder to be used
         * @return reference to this generator for method chaining
         */
        ShapeGenerator& VertexBufferBuilder(
            std::shared_ptr<VertexBufferBuilder2f> builder);

        /**
         * Specifies the offset from the center used to generate shapes.
         * 
         * @param o the offset
         * @return reference to this generator for method chaining
         */
        ShapeGenerator& Offset(const Vector2f o) {
            offset = o;
            return *this;
        }

        /**
         * Specifies the offset from the center used to generate shapes.
         * 
         * @param ox    the x-coordinate of the offset
         * @param oy    the y-coordinate of the offset
         * @return reference to this generator for method chaining
         */
        ShapeGenerator& Offset(float ox, float oy) {
            offset.Set(ox, oy);
            return *this;
        }

        /**
         * Swichtes this shape generator to polyline mode.
         * 
         * Polyline mode will close the shaped by duplicating
         * the first vertex.
         */
        ShapeGenerator& PolygoneMode() {
            duplicateStartVertex = false;
            return *this;
        }

        /**
         * Swichtes this shape generator to polygon mode.
         * 
         * Polyline mode asumes that the created polygone is closed
         * automatically.
         */
        ShapeGenerator& PolylineMode() {
            duplicateStartVertex = true;
            return *this;
        }

        /**
         * Returns whether this generator is in polyline mode.
         * 
         * @return `true` if this generator is in polyline mode
         */
        bool IsPolylineMode() const {
            return duplicateStartVertex;
        }

        /**
         * Returns whether this generator is in polygon mode.
         * 
         * @return `true` if this generator is in polygon mode
         */
        bool IsPolygoneMode() const {
            return !duplicateStartVertex;
        }

        /**
         * Generates a circle.
         * 
         * @param r the radius of the circle
         * @param n the number of segments
         */
        std::shared_ptr<VertexBuffer2f> GenCircle(
            float r, unsigned int n = 24) const;

        /**
         * Generates an axis aligned rectangle.
         * 
         * @param w the width of the rectangle
         * @param h the height of the rectangle
         */
        std::shared_ptr<VertexBuffer2f> GenRectangle(float w, float h) const;

        /**
         * Generates an axis aligned rectangle.
         * 
         * @param a the side length of the square.
         */
        std::shared_ptr<VertexBuffer2f> GenSquare(float a) const {
            return GenRectangle(a, a);
        }

        /**
         * Generates an equilateral triangle.
         * 
         * @param r the radius of the equilateral triangle
         * @param d the direction in which the triangle points
         */
        std::shared_ptr<VertexBuffer2f> GenTriangle(
            float r, const Vector2f & d = Vector2f(0, -1));

        /**
         * Generates a star.
         * 
         * @param r the radius of the star.
         * @param n determines the number of point the star should have
         * @param d the direction in which the star points
         */
        std::shared_ptr<VertexBuffer2f> GenStar(
            float r, 
            int n = 5, 
            const Vector2f & d = Vector2f(0, -1)
        );

        /**
         * Generates a cross.
         * 
         * The thickness paramter must be within in the range (0, 1).
         * 
         * @param s     the size of the cross
         * @param th    determines the thickness of the cross (0, 1)
         */
        std::shared_ptr<VertexBuffer2f> GenCross(float s, float th = 0.381967f);

        /**
         * Generates an arrow.
         * 
         * The thickness paramter must be within in the range (0, 1).
         * 
         * @param l     the length of the arrow
         * @param th    determines the thickness of the arrow (0, 1)
         * @param d     the direction in which the arrow points
         */
        std::shared_ptr<VertexBuffer2f> GenArrow(
            float l, float th = 0.381967f, const Vector2f & d = Vector2f(1, 0));

        /**
         * Resets this generator to its initial configuration.
         * 
         * @return reference to this generator for method chaining
         */
        ShapeGenerator& Reset();

    private:
        /** The vertex buffer builder used to generate the shapes. */
        std::shared_ptr<VertexBufferBuilder2f> vbBuilder;

        /** The offset from the center used to generate the shapes. */
        Vector2f offset;

        /** Whether to duplicate the start vertex, used for closed polylines. */
        bool duplicateStartVertex;

        /** 
         * Returns the vertex buffer builder to be used.
         * 
         * @return the vertex buffer builder
         * @throws std::runtime_error in case no VertexBuffer2Builder exists
         */
        VertexBufferBuilder2f& GetBuilder() const;
    };

} // end of namespace