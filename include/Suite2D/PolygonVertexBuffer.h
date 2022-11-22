/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Polygon.h"
#include "Math/Vector2.h"
#include "Graphics/VertexBuffer2.h"

// C++ Standard Library includes
#include <vector>
#include <memory>

namespace astu {

    /**
     * Polygon vertex buffer builder.
     */
    template <typename T>
    class PolygonVertexBufferBuilder : public VertexBufferBuilder2<T> {
    public:

        /**
         * Constructor.
         */
        PolygonVertexBufferBuilder() {
            Reset();
        }

        // Inherited via VertexBufferBuilder2D
        virtual VertexBufferBuilder2<T>& AddVertex(T x, T y) override {
            vertices.push_back(Vector2<T>(x, y));
            return *this;
        }

        virtual size_t GetNumVertices() const override {
            return vertices.size();
        }

        virtual const Vector2<T> & GetVertex(size_t idx) const override {
            return vertices.at(idx);
        }

        virtual VertexBufferBuilder2<T>& SetVertex(size_t idx, T x, T y) override {
            vertices.at(idx) = Vector2<T>(x, y);
            return *this;
        }

        virtual VertexBufferBuilder2<T>& Reset() override {
            vertices.clear();
            return *this;
        }

        virtual std::shared_ptr<VertexBuffer2<T>> Build() override {
            return std::make_shared<Polygon<T>>(vertices);
        }       

    private:
        /** The vertices used for the vertex buffer to build. */
        std::vector<Vector2<T>> vertices;
    };

    using PolygonVertexBufferBuilder2d = astu::PolygonVertexBufferBuilder<double>;    
    using PolygonVertexBufferBuilder2f = astu::PolygonVertexBufferBuilder<float>;    


} // end of namespace