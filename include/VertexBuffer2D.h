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
#include "Vector2.h"

namespace astu {

    /**
     * Abstract base class for vertex buffers.
     */
    class VertexBuffer2D {
    public:

        /** Virtual destructor. */
        virtual ~VertexBuffer2D() {}
    };

    /**
     * Abstract base class for vertex buffer buildes.
     */
    class VertexBufferBuilder2D {
    public:

        /** Virtual destructor. */
        virtual ~VertexBufferBuilder2D() {}

        /**
         * Adds the specified vertex.
         * 
         * @param x the x-coordinate of the vertex
         * @param y the y-coordinate of the vertex
         * @return reference to this builder for method chaining
         */
        virtual VertexBufferBuilder2D& AddVertex(float x, float y) = 0;

        /**
         * Adds the specified vertex.
         * 
         * @param vertex    the vertex to add
         * @return reference to this builder for method chaining
         */
        VertexBufferBuilder2D& AddVertex(const Vector2f& vertex) {
            return AddVertex(vertex.x, vertex.y);
        }

        /**
         * Returns the number of vertices, added to this builder.
         * 
         * @return the number of vertices
         */
        virtual size_t GetNumVertices() const = 0;

        /**
         * Returns the vertex with the specified index.
         * 
         * @param idx   the index of the vertex to return
         * 
         * @return the vertex
         * @throws std::out_of_range in case the specified index is invalid
         */
        virtual const Vector2f & GetVertex(size_t idx) const = 0;

        /**
         * Redefines a previously added vertex.
         * 
         * @param idx       the index of the vertex to redefine
         * @param vertex    the vertex
         */
        virtual void SetVertex(size_t idx, const Vector2f& vertex) {
            SetVertex(idx, vertex.x, vertex.y);
        }

        /**
         * Redefines a previously added vertex.
         * 
         * @param idx       the index of the vertex to redefine
         * @param x the x-coordinate of the vertex
         * @param y the y-coordinate of the vertex
         * @throws std::out_of_range in case the specified index is invalid
         */
        virtual void SetVertex(size_t idx, float x, float y) = 0;

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        virtual VertexBufferBuilder2D& Reset() = 0;

        /**
         * Creates a new vertex buffer.
         * 
         * @return the newly created vertex buffer
         */
        virtual std::shared_ptr<VertexBuffer2D> Build() = 0;        
    };

} // end of namespace