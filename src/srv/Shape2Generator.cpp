/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include "MathUtils.h"
#include "Vector2.h"
#include "Service.h"

// Local includes
#include "Shape2Generator.h"

using namespace std;

namespace astu {

    Shape2Generator::Shape2Generator()
    {
        // Intentionally left empty.        
    }

    Shape2Generator& Shape2Generator::VertexBufferBuilder(shared_ptr<VertexBuffer2Builder> builder)
    {
        vbBuilder = builder;
        return *this;
    }

    shared_ptr<VertexBuffer2> Shape2Generator::GenCircle(float r, unsigned int n) const
    {   
        auto & builder = GetBuilder();

        builder.Reset();
        for (unsigned int i = 0; i < n; ++i) {
            Vector2f p(r, 0);
            p.Rotate(i * MathUtils::PI2f / n);
            builder.AddVertex(p + offset);
        }
        builder.AddVertex(Vector2f(r, 0) + offset);
        
        return builder.Build();
    }

    std::shared_ptr<VertexBuffer2> Shape2Generator::GenRectangle(float w, float h) const
    {
        auto & builder = GetBuilder();
        builder.Reset();
        builder.AddVertex(Vector2f(-w / 2, -h / 2) + offset);
        builder.AddVertex(Vector2f(w / 2, -h / 2) + offset);
        builder.AddVertex(Vector2f(w / 2, h / 2) + offset);
        builder.AddVertex(Vector2f(-w / 2, h / 2) + offset);
        builder.AddVertex(Vector2f(-w / 2, -h / 2) + offset);

        return builder.Build();
    }

    Shape2Generator& Shape2Generator::Reset()
    {
        vbBuilder = nullptr;
        offset.SetZero();
        return *this;
    }

    VertexBuffer2Builder& Shape2Generator::GetBuilder() const
    {
        if (vbBuilder) {
            return *vbBuilder;
        }

        return ASTU_SERVICE(VertexBuffer2Builder);
    }

} // end of namespace