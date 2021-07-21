/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "ShapeGenerator2D.h"

// C++ Standard Library.
#include <stdexcept>
#include <cmath>

// AST Utilities includes
#include "MathUtils.h"
#include "Vector2.h"
#include "Service.h"

using namespace std;

namespace astu {

    ShapeGenerator2D::ShapeGenerator2D()
    {
        Reset();
    }

    ShapeGenerator2D& ShapeGenerator2D::VertexBufferBuilder(shared_ptr<VertexBufferBuilder2f> builder)
    {
        vbBuilder = builder;
        return *this;
    }

    shared_ptr<VertexBuffer2f> ShapeGenerator2D::GenCircle(float r, unsigned int n) const
    {   
        if (r <= 0) {
            throw domain_error(
                "Unable to generate circle, radius is less or equal zero");
        }

        auto & builder = GetBuilder();
        builder.Reset();

        for (unsigned int i = 0; i < n; ++i) {
            Vector2f p(r, 0);
            p.Rotate(i * -MathUtils::PI2f / n);
            builder.AddVertex(p + offset);
        }
        if (duplicateStartVertex) {
            builder.AddVertex(Vector2f(r, 0) + offset);
        }
        
        return builder.Build();
    }

    shared_ptr<VertexBuffer2f> ShapeGenerator2D::GenRectangle(float w, float h) const
    {
        auto & builder = GetBuilder();
        builder.Reset();
        builder.AddVertex(Vector2f(-w / 2, -h / 2) + offset);
        builder.AddVertex(Vector2f(-w / 2, h / 2) + offset);
        builder.AddVertex(Vector2f(w / 2, h / 2) + offset);
        builder.AddVertex(Vector2f(w / 2, -h / 2) + offset);
        if (duplicateStartVertex) {
            builder.AddVertex(Vector2f(-w / 2, -h / 2) + offset);
        }

        return builder.Build();
    }

    shared_ptr<VertexBuffer2f> ShapeGenerator2D::GenTriangle(float r, const Vector2f & d)
    {
        if (d.IsZero()) {
            throw domain_error(
                "Unable to generate triangle, degenerated direction vector");
        }
        if (r <= 0) {
            throw domain_error(
                "Unable to generate triangle, radius is less or equal zero");
        }

        auto & builder = GetBuilder();
        builder.Reset();

        Vector2f ref = d;
        ref.SetLength(r);
        for (unsigned int i = 0; i < 3; ++i) {
            Vector2f p = ref;
            p.Rotate(i * -MathUtils::PI2f / 3);
            builder.AddVertex(p + offset);
        }
        if (duplicateStartVertex) {
            builder.AddVertex(ref + offset);
        }
        
        return builder.Build();
    }

    shared_ptr<VertexBuffer2f> ShapeGenerator2D::GenStar(float r, int n, const Vector2f & d)
    {
        if (d.IsZero()) {
            throw domain_error(
                "Unable to generate star, degenerated direction vector");
        }

        if (r <= 0) {
            throw domain_error(
                "Unable to generate star, radius is less or equal zero");
        }

        if (n < 4) {
            throw domain_error(
                "Unable to generate star, number of points musst be greater or equal four");
        }


        auto & builder = GetBuilder();
        builder.Reset();

        Vector2f ref = d;
        ref.SetLength(r);
        const int steps = n * 2;
        for (int i = 0; i < steps; ++i) {
            Vector2f p = ref;
            if(i % 2 != 0) {
                p *= 0.5f;
            }
            p.Rotate(i * -MathUtils::PI2f / steps);
            builder.AddVertex(p + offset);
        }
        if (duplicateStartVertex) {
            builder.AddVertex(ref + offset);
        }
        
        return builder.Build();
    }

    shared_ptr<VertexBuffer2f> ShapeGenerator2D::GenCross(float s, float th)
    {
        if (th <= 0 || th >= 1) {
            throw domain_error(
                "Unable to generate cross, thickness parameter is out of range (0, 1)");
        }
        auto & builder = GetBuilder();
        builder.Reset();

        float s2 = s * 0.5f;
        float s4 = s2 * th;
        builder.AddVertex(Vector2f(-s2, -s4) + offset);
        builder.AddVertex(Vector2f(-s2,  s4) + offset);
        builder.AddVertex(Vector2f(-s4,  s4) + offset);
        builder.AddVertex(Vector2f(-s4,  s2) + offset);
        builder.AddVertex(Vector2f( s4,  s2) + offset);
        builder.AddVertex(Vector2f( s4,  s4) + offset);
        builder.AddVertex(Vector2f( s2,  s4) + offset);
        builder.AddVertex(Vector2f( s2, -s4) + offset);
        builder.AddVertex(Vector2f( s4, -s4) + offset);
        builder.AddVertex(Vector2f( s4, -s2) + offset);
        builder.AddVertex(Vector2f(-s4, -s2) + offset);
        builder.AddVertex(Vector2f(-s4, -s4) + offset);
 
        if (duplicateStartVertex) {
            builder.AddVertex(Vector2f(-s2, -s4) + offset);
        }

        return builder.Build();
    }

    shared_ptr<VertexBuffer2f> ShapeGenerator2D::GenArrow(float s, float th, const Vector2f & d)
    {
        auto & builder = GetBuilder();
        builder.Reset();

        float w2 = s * 0.5f;
        float a = s * th;
        float a2 = a * 0.5f;
        float h = 1.73205081f * a2;
        float h2 = h * 0.5f;
        float y2 = a2 * 0.618033f;

        builder.AddVertex(Vector2f(-w2, -y2) + offset);
        builder.AddVertex(Vector2f(-w2, y2) + offset);
        builder.AddVertex(Vector2f(w2 - h, y2) + offset);
        builder.AddVertex(Vector2f(w2 - h, a2) + offset);
        builder.AddVertex(Vector2f(w2, 0) + offset);
        builder.AddVertex(Vector2f(w2 - h, -a2) + offset);
        builder.AddVertex(Vector2f(w2 - h, -y2) + offset);

        if (duplicateStartVertex) {
            builder.AddVertex(Vector2f(-w2, -y2) + offset);
        }

        return builder.Build();
    }

    ShapeGenerator2D& ShapeGenerator2D::Reset()
    {
        vbBuilder = nullptr;
        offset.SetZero();
        duplicateStartVertex = true;
        return *this;
    }

    VertexBufferBuilder2f& ShapeGenerator2D::GetBuilder() const
    {
        if (vbBuilder) {
            return *vbBuilder;
        }

        return ASTU_SERVICE(VertexBufferBuilder2f);
    }

} // end of namespace