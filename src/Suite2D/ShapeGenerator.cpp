/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Suite2D/ShapeGenerator.h"
#include "Service/Service.h"
#include "Math/MathUtils.h"
#include "Math/Vector2.h"

// C++ Standard Library.
#include <stdexcept>
#include <cmath>

using namespace std;

#define RESET_VERTICES() tempVertices.clear()
#define ADD_VERTEX(v) tempVertices.push_back(v)
#define GET_VERTICES() tempVertices

namespace astu::suite2d {

    std::vector<Vector2f> ShapeGenerator::tempVertices;

    ShapeGenerator::ShapeGenerator()
    {
        Reset();
    }

    ShapeGenerator& ShapeGenerator::VertexBufferBuilder(shared_ptr<VertexBufferBuilder2f> builder)
    {
        vbBuilder = builder;
        return *this;
    }

    const std::vector<Vector2f>& ShapeGenerator::GenCircle(float r, unsigned int n) const
    {   
        if (r <= 0) {
            throw domain_error(
                "Unable to generate circle, radius is less or equal zero");
        }

        RESET_VERTICES();

        for (unsigned int i = 0; i < n; ++i) {
            Vector2f p(r, 0);
            p.Rotate(i * -MathUtils::PI2f / n);
            ADD_VERTEX(p + offset);
        }
        if (duplicateStartVertex) {
            ADD_VERTEX(Vector2f(r, 0) + offset);
        }

        return GET_VERTICES();
    }

    const std::vector<Vector2f>& ShapeGenerator::GenRectangle(float w, float h) const
    {
        RESET_VERTICES();

        ADD_VERTEX(Vector2f(-w / 2, -h / 2) + offset);
        ADD_VERTEX(Vector2f(-w / 2, h / 2) + offset);
        ADD_VERTEX(Vector2f(w / 2, h / 2) + offset);
        ADD_VERTEX(Vector2f(w / 2, -h / 2) + offset);
        if (duplicateStartVertex) {
            ADD_VERTEX(Vector2f(-w / 2, -h / 2) + offset);
        }

        return GET_VERTICES();
    }

    const std::vector<Vector2f>& ShapeGenerator::GenTriangle(float r, const Vector2f & d)
    {
        if (d.IsZero()) {
            throw domain_error(
                "Unable to generate triangle, degenerated direction vector");
        }
        if (r <= 0) {
            throw domain_error(
                "Unable to generate triangle, radius is less or equal zero");
        }

        RESET_VERTICES();

        Vector2f ref = d;
        ref.SetLength(r);
        for (unsigned int i = 0; i < 3; ++i) {
            Vector2f p = ref;
            p.Rotate(i * -MathUtils::PI2f / 3);
            ADD_VERTEX(p + offset);
        }
        if (duplicateStartVertex) {
            ADD_VERTEX(ref + offset);
        }
        
        return GET_VERTICES();
    }

    const std::vector<Vector2f>& ShapeGenerator::GenStar(float r, int n, const Vector2f & d)
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

        RESET_VERTICES();

        Vector2f ref = d;
        ref.SetLength(r);
        const int steps = n * 2;
        for (int i = 0; i < steps; ++i) {
            Vector2f p = ref;
            if(i % 2 != 0) {
                p *= 0.5f;
            }
            p.Rotate(i * -MathUtils::PI2f / steps);
            ADD_VERTEX(p + offset);
        }
        if (duplicateStartVertex) {
            ADD_VERTEX(ref + offset);
        }
        
        return GET_VERTICES();
    }

    const std::vector<Vector2f>& ShapeGenerator::GenCross(float s, float th)
    {
        if (th <= 0 || th >= 1) {
            throw domain_error(
                "Unable to generate cross, thickness parameter is out of range (0, 1)");
        }

        RESET_VERTICES();

        float s2 = s * 0.5f;
        float s4 = s2 * th;
        ADD_VERTEX(Vector2f(-s2, -s4) + offset);
        ADD_VERTEX(Vector2f(-s2,  s4) + offset);
        ADD_VERTEX(Vector2f(-s4,  s4) + offset);
        ADD_VERTEX(Vector2f(-s4,  s2) + offset);
        ADD_VERTEX(Vector2f( s4,  s2) + offset);
        ADD_VERTEX(Vector2f( s4,  s4) + offset);
        ADD_VERTEX(Vector2f( s2,  s4) + offset);
        ADD_VERTEX(Vector2f( s2, -s4) + offset);
        ADD_VERTEX(Vector2f( s4, -s4) + offset);
        ADD_VERTEX(Vector2f( s4, -s2) + offset);
        ADD_VERTEX(Vector2f(-s4, -s2) + offset);
        ADD_VERTEX(Vector2f(-s4, -s4) + offset);
 
        if (duplicateStartVertex) {
            ADD_VERTEX(Vector2f(-s2, -s4) + offset);
        }

        return GET_VERTICES();
    }

    const std::vector<Vector2f>& ShapeGenerator::GenArrow(float s, float th, const Vector2f & d)
    {
        RESET_VERTICES();

        float w2 = s * 0.5f;
        float a = s * th;
        float a2 = a * 0.5f;
        float h = 1.73205081f * a2;
        float h2 = h * 0.5f;
        float y2 = a2 * 0.618033f;

        ADD_VERTEX(Vector2f(-w2, -y2) + offset);
        ADD_VERTEX(Vector2f(-w2, y2) + offset);
        ADD_VERTEX(Vector2f(w2 - h, y2) + offset);
        ADD_VERTEX(Vector2f(w2 - h, a2) + offset);
        ADD_VERTEX(Vector2f(w2, 0) + offset);
        ADD_VERTEX(Vector2f(w2 - h, -a2) + offset);
        ADD_VERTEX(Vector2f(w2 - h, -y2) + offset);

        if (duplicateStartVertex) {
            ADD_VERTEX(Vector2f(-w2, -y2) + offset);
        }

        return GET_VERTICES();
    }

    ShapeGenerator& ShapeGenerator::Reset()
    {
        vbBuilder = nullptr;
        offset.SetZero();
        duplicateStartVertex = true;
        return *this;
    }

    VertexBufferBuilder2f& ShapeGenerator::GetBuilder() const
    {
        if (vbBuilder) {
            return *vbBuilder;
        }

        return ASTU_SERVICE(VertexBufferBuilder2f);
    }

} // end of namespace