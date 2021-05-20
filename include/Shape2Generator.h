/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>

// Local includes
#include "VertexBuffer2.h"

namespace astu {

    /**
     * Utility class used to generate two-dimensional basic geometric shapes.
     */
    class Shape2Generator final {
    public:

        /**
         * Constructor.
         */
        Shape2Generator();

        /**
         * Specifies the vertex buffer builder to be used.
         * 
         * @builder the vertex buffer builder to be used
         * @return reference to this generator for method chaining
         */
        Shape2Generator& VertexBufferBuilder(
            std::shared_ptr<VertexBuffer2Builder> builder);

        /**
         * Specifies the offset from the center used to generate shapes.
         * 
         * @param o the offset
         * @return reference to this generator for method chaining
         */
        Shape2Generator& Offset(const Vector2f o) {
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
        Shape2Generator& Offset(float ox, float oy) {
            offset.Set(ox, oy);
            return *this;
        }

        /**
         * Generates a circle.
         * 
         * @param r the radius of the circle
         * @param n the number of segments
         */
        std::shared_ptr<VertexBuffer2> GenCircle(
            float r, unsigned int n = 24) const;

        /**
         * Generates an axis aligned rectangle.
         * 
         * @param w the width of the rectangle
         * @param n the height of the rectangle
         */
        std::shared_ptr<VertexBuffer2> GenRectangle(float w, float h) const;

        /**
         * Generates an axis aligned rectangle.
         * 
         * @param a the side length of the square.
         */
        std::shared_ptr<VertexBuffer2> GenSquare(float a) const {
            return GenRectangle(a, a);
        }

        /**
         * Generates an equilateral triangle.
         * 
         * @param r the radius of the equilateral triangle
         * @param d the direction in which the triangle points
         */
        std::shared_ptr<VertexBuffer2> GenTriangle(
            float r, const Vector2f & d = Vector2f(0, -1));

        /**
         * Generates a star.
         * 
         * @param r the radius of the star.
         * @param n determines the number of point the star should have
         * @param d the direction in which the star points
         */
        std::shared_ptr<VertexBuffer2> GenStar(
            float r, 
            int n = 5, 
            const Vector2f & d = Vector2f(0, -1)
        );

        /**
         * Generates a cross.
         * 
         * The thickness paramter must be within in the range (0, 1).
         * 
         * @param r     the size of the cross
         * @param th    determines the thickness of the cross (0, 1)
         */
        std::shared_ptr<VertexBuffer2> GenCross(float s, float th = 0.381967f);

        /**
         * Generates an arrow.
         * 
         * The thickness paramter must be within in the range (0, 1).
         * 
         * @param l     the length of the arrow
         * @param th    determines the thickness of the arrow (0, 1)
         * @param d     the direction in which the arrow points
         */
        std::shared_ptr<VertexBuffer2> GenArrow(
            float l, float th = 0.381967f, const Vector2f & d = Vector2f(1, 0));


        /**
         * Resets this generator to its initial configuration.
         * 
         * @return reference to this generator for method chaining
         */
        Shape2Generator& Reset();

    private:
        /** The vertex buffer builder used to generate the shapes. */
        std::shared_ptr<VertexBuffer2Builder> vbBuilder;

        /** The offset from the center used to generate the shapes. */
        Vector2f offset;

        /** 
         * Returns the vertex buffer builder to be used.
         * 
         * @return the vertex buffer builder
         * @throws std::runtime_error in case no VertexBuffer2Builder exists
         */
        VertexBuffer2Builder& GetBuilder() const;
    };

} // end of namespace