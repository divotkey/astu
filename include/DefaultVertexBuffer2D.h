/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "VertexBuffer2D.h"
#include "Vector2.h"

// C++ Standard Library includes
#include <vector>

namespace astu {

    /**
     * Default vertex buffers.
     */
    class DefaultVertexBuffer2D : public VertexBuffer2D {
    public:

        DefaultVertexBuffer2D(const std::vector<Vector2f> vertices)
            : vertices(vertices) {}

        /** Virtual destructor. */
        virtual ~DefaultVertexBuffer2D() {}

        std::vector<Vector2f> vertices;
    };

    /**
     * Default vertex buffer builder.
     */
    class DefaultVertexBufferBuilder2D : public VertexBufferBuilder2D {
    public:

        /**
         * Constructor.
         */
        DefaultVertexBufferBuilder2D();

        /** Virtual destructor. */
        virtual ~DefaultVertexBufferBuilder2D() {}

        // Inherited via VertexBufferBuilder2D
        virtual VertexBufferBuilder2D& AddVertex(float x, float y) override;
        virtual size_t GetNumVertices() const override;
        virtual const Vector2f & GetVertex(size_t idx) const override;
        virtual void SetVertex(size_t idx, float x, float y) override;
        virtual VertexBufferBuilder2D& Reset() override;
        virtual std::shared_ptr<VertexBuffer2D> Build() override;        

    private:
        /** The vertices used for the vertex buffer to build. */
        std::vector<Vector2f> vertices;
    };

} // end of namespace