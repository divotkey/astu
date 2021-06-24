/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <iostream>
#include <cassert>

// Simple Direct Layer includes
#include <SDL2/SDL.h>

// Local includes
#include "SdlScene2Renderer.h"

using namespace std;

#define ASSERT_VBUF(a) assert(dynamic_cast<SdlVertexBuffer2*>(&a))
#define VBUF(a) static_cast<const SdlVertexBuffer2&>(a)

namespace astu {
    
    /////////////////////////////////////////////////
    /////// SdlScene2Renderer
    /////////////////////////////////////////////////

    SdlScene2Renderer::SdlScene2Renderer()
        : renderer(nullptr)
    {
        // Intentionally left empty.
    }

    void SdlScene2Renderer::Render(Polyline2& polyline)
    {
        ASSERT_VBUF(polyline.GetVertexBuffer());
        assert(renderer);

        const auto & vertices = VBUF(polyline.GetVertexBuffer()).vertices;
        if (vertices.size() < 2) {
            return;
        }

        const auto& c = polyline.GetColor();
        SDL_SetRenderDrawColor(
            renderer, 
            static_cast<int>(c.r * 255), 
            static_cast<int>(c.g * 255), 
            static_cast<int>(c.b * 255), 
            static_cast<int>(c.a * 255)
            );

        const auto & tx = viewMatrix * polyline.GetWorldMatrix();
        auto it = vertices.cbegin();
        auto p1 = tx.TransformPoint(*it);

        while (++it != vertices.cend()) {
            const auto p2 = tx.TransformPoint(*it);
            SDL_RenderDrawLine(
                renderer, 
                static_cast<int>(p1.x), 
                static_cast<int>(p1.y), 
                static_cast<int>(p2.x), 
                static_cast<int>(p2.y)
                );

            p1 = p2;
        }
    }

} // end of namespace
