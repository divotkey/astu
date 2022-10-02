/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SdlSceneRenderer2D.h"
#include "SdlTexture.h"

// Simple Direct Layer includes
#include <SDL2/SDL.h>

// C++ Standard Library includes
#include <iostream>
#include <cassert>

using namespace std;
using namespace astu::suite2d;

#define ASSERT_VBUF(a) assert(dynamic_cast<SdlVertexBuffer2D*>(&a))
#define ASSERT_TEXTURE(a) assert(dynamic_cast<SdlTexture*>(&a))
#define VBUF(a) static_cast<const SdlVertexBuffer2D&>(a)
#define TEXTURE(a) static_cast<SdlTexture&>(a)
#define SDL_TEXTURE(a) TEXTURE(a).GetSdlTexture()

namespace astu {
    
    /////////////////////////////////////////////////
    /////// SdlScene2Renderer
    /////////////////////////////////////////////////

    SdlSceneRenderer2D::SdlSceneRenderer2D()
        : renderer(nullptr)
    {
        // Intentionally left empty.
    }

    void SdlSceneRenderer2D::Render(Polyline& polyline, float alpha)
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
            static_cast<int>((c.a * alpha) * 255)
            );

        const auto & tx = viewMatrix * polyline.GetWorldMatrix();
        auto it = vertices.cbegin();
        auto first = tx.TransformPoint(*it);
        auto p1 = first;

        while (++it != vertices.cend()) {
            const auto p2 = tx.TransformPoint(*it);
            SDL_RenderDrawLine(
                renderer, 
                static_cast<int>(p1.x + 0.5f), 
                static_cast<int>(p1.y + 0.5f), 
                static_cast<int>(p2.x + 0.5f), 
                static_cast<int>(p2.y + 0.5f)
                );

            p1 = p2;
        }

        if (polyline.IsClosed()) {
            SDL_RenderDrawLine(
                renderer, 
                static_cast<int>(p1.x + 0.5f), 
                static_cast<int>(p1.y + 0.5f), 
                static_cast<int>(first.x + 0.5f), 
                static_cast<int>(first.y + 0.5f)
                );
        }
    }

    void SdlSceneRenderer2D::Render(Sprite &sprite, float alpha)
    {
        assert(renderer);
        ASSERT_TEXTURE(sprite.GetTexture());

        const auto & tx = viewMatrix * sprite.GetWorldMatrix();

        auto topLeft = tx.TransformPoint(Vector2f(-sprite.GetWidth() / 2, -sprite.GetHeight() / 2));
        auto topRight = tx.TransformPoint(Vector2f(sprite.GetWidth() / 2, -sprite.GetHeight() / 2));
        auto bottomLeft = tx.TransformPoint(Vector2f(-sprite.GetWidth() / 2, sprite.GetHeight() / 2));
        auto bottomRight = tx.TransformPoint(Vector2f(sprite.GetWidth() / 2, sprite.GetHeight() / 2));

        SDL_Rect dstRect;
        dstRect.x = static_cast<int>(topLeft.x + 0.5f);
        dstRect.y = static_cast<int>(topLeft.y + 0.5f);
        dstRect.w = static_cast<int>(topRight.Distance(topLeft) + 0.5f);
        dstRect.h = static_cast<int>(bottomLeft.Distance(topLeft) + 0.5f);

        SDL_Point pivot = {0, 0};

        SDL_RenderCopyEx(
                renderer,
                TEXTURE(sprite.GetTexture()),
                nullptr,
                &dstRect,
                (bottomRight - bottomLeft).AngleDeg(),
                &pivot,
                SDL_RendererFlip::SDL_FLIP_NONE
                );
    }

} // end of namespace
