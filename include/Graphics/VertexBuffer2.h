/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"

// C++ Standard Library includes
#include <memory>

namespace astu {

    /**
     * Abstract base class for vertex buffers.
     */
    template <typename T>
    class VertexBuffer2 {
    public:

        /** Virtual destructor. */
        virtual ~VertexBuffer2() {}
    };
    
    using VertexBuffer2d = astu::VertexBuffer2<double>;    
    using VertexBuffer2f = astu::VertexBuffer2<float>;    

    /**
     * Abstract base class for vertex buffer builders.
     */
    template <typename T>
    class VertexBufferBuilder2 {
    public:

        /** Virtual destructor. */
        virtual ~VertexBufferBuilder2() {}

        /**
         * Adds the specified vertex.
         * 
         * @param x the x-coordinate of the vertex
         * @param y the y-coordinate of the vertex
         * @return reference to this builder for method chaining
         */
        virtual VertexBufferBuilder2& AddVertex(T x, T y) = 0;

        /**
         * Adds the specified vertex.
         * 
         * @param vertex    the vertex to add
         * @return reference to this builder for method chaining
         */
        VertexBufferBuilder2& AddVertex(const Vector2<T>& vertex) {
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
        virtual const Vector2<T> & GetVertex(size_t idx) const = 0;

        /**
         * Redefines a previously added vertex.
         * 
         * @param idx       the index of the vertex to redefine
         * @param vertex    the vertex
         * @return reference to this builder for method chaining
         */
        virtual VertexBufferBuilder2& SetVertex(size_t idx, const Vector2<T>& vertex) {
            return SetVertex(idx, vertex.x, vertex.y);
        }

        /**
         * Redefines a previously added vertex.
         * 
         * @param idx       the index of the vertex to redefine
         * @param x the x-coordinate of the vertex
         * @param y the y-coordinate of the vertex
         * @return reference to this builder for method chaining
         * @throws std::out_of_range in case the specified index is invalid
         */
        virtual VertexBufferBuilder2& SetVertex(size_t idx, T x, T y) = 0;

        /**
         * Resets this builder to its initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        virtual VertexBufferBuilder2& Reset() = 0;

        /**
         * Creates a new vertex buffer.
         * 
         * @return the newly created vertex buffer
         */
        virtual std::shared_ptr<VertexBuffer2<T>> Build() = 0;        
    };

    using VertexBufferBuilder2d = astu::VertexBufferBuilder2<double>;    
    using VertexBufferBuilder2f = astu::VertexBufferBuilder2<float>;    

} // end of namespace