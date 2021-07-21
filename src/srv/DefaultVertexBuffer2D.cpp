/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "DefaultVertexBuffer2D.h"

namespace astu {

    DefaultVertexBufferBuilder2D::DefaultVertexBufferBuilder2D()
    {
        Reset();
    }

    VertexBufferBuilder2f& DefaultVertexBufferBuilder2D::AddVertex(float x, float y)
    {
        vertices.push_back(Vector2f(x, y));
        return *this;
    }

    size_t DefaultVertexBufferBuilder2D::GetNumVertices() const
    {
        return vertices.size();
    }

    const Vector2f& DefaultVertexBufferBuilder2D::GetVertex(size_t idx) const
    {
        return vertices.at(idx);
    }

    VertexBufferBuilder2f& DefaultVertexBufferBuilder2D::SetVertex(size_t idx, float x, float y)
    {
        vertices.at(idx).Set(x, y);
        return *this;
    }

    VertexBufferBuilder2f& DefaultVertexBufferBuilder2D::Reset()
    {
        vertices.clear();
        return *this;
    }

    std::shared_ptr<VertexBuffer2f> DefaultVertexBufferBuilder2D::Build()
    {
        return std::make_shared<DefaultVertexBuffer2D>(vertices);
    }

} // end of namespace