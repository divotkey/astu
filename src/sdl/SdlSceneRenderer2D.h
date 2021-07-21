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
#include "Scene2D.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "VertexBuffer2D.h"

// Forward declaration.
struct SDL_Renderer;

namespace astu {

    class SdlVertexBuffer2D : public VertexBuffer2f {
    public:
        std::vector<Vector2f> vertices;
    };

    class SdlSceneRenderer2D : public SceneRenderer2D {
    public:

        /**
         * Constructor.
         */
        SdlSceneRenderer2D();

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
        virtual void Render(Polyline2D& polyline, float alpha) override;

    private:
        /** The SDL renderer used for rendering. */
        SDL_Renderer *renderer;

        /** The view transformation. */
        Matrix3f viewMatrix;
    };

} // end of namespace