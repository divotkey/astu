/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard libraries includes
#include <vector>

// Local includes
#include "Scene2.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "VertexBuffer2.h"

// Forward declaration.
struct SDL_Renderer;

namespace astu {

    class SdlVertexBuffer2 : public VertexBuffer2 {
    public:
        std::vector<Vector2f> vertices;
    };

    class SdlScene2Renderer : public Scene2Renderer {
    public:

        /**
         * Constructor.
         */
        SdlScene2Renderer();

        /**
         * Specifies the SDL renderer used for subsequent render calls.
         * 
         * @param renderer  the SDL renderer
         */
        void SetSdlRenderer(SDL_Renderer &renderer) {
            this->renderer = &renderer;
        }

        /**
         * Releases the specified SDL renderer.
         */
        void ClearSdlRenderer() {
            renderer = nullptr;
        }

        /**
         * Sets the view transformation.
         * 
         * @param m the transformation matrix
         */
        void SetViewMatrix(const Matrix3f& m) {
            viewMatrix = m;
        }

        // Inherited via Scene2Renderer
        virtual void Render(Polyline2& polyline, float alpha) override;

    private:
        /** The SDL renderer used for rendering. */
        SDL_Renderer *renderer;

        /** The view transformation. */
        Matrix3f viewMatrix;
    };

} // end of namespace