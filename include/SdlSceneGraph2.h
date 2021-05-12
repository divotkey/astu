/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard libraries includes
#include <vector>
#include <cstdint>

// Local includes
#include "Scene2.h"
#include "Vector2.h"
#include "VertexBuffer2.h"
#include "SdlRenderService.h"

namespace astu {

    // Forward declaration

    class SdlVertexBuffer2 : public VertexBuffer2 {
    public:
        std::vector<Vector2f> vertices;
    };

    class SdlSceneGraph2 final : public SdlRenderLayer, public SceneGraph2 {
    public:

        /**
         * Constructor.
         * 
         * @param renderPriority    the priority of this render layer
         */
        SdlSceneGraph2(int renderPriority = 0);

        // Inherited via SdlRenderLayer
        virtual void OnRender(SDL_Renderer* renderer) override;
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:

    };

} // end of namespace