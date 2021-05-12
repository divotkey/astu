/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <cassert>     
#include <SDL2/SDL.h>

// Local (AST-Utilities) includes
#include "SdlSceneGraph2.h"

namespace astu {

    SdlSceneGraph2::SdlSceneGraph2(int renderPriority)
        : Service("SDL Scene Graph (2D)")
        , SdlRenderLayer(renderPriority)
    {
        // Intentionally left empty.
    }

    void SdlSceneGraph2::OnRender(SDL_Renderer* renderer)
    {

    }

    void SdlSceneGraph2::OnStartup()
    {
        // Intentionally left empty.
    }

    void SdlSceneGraph2::OnShutdown()
    {
        // Intentionally left empty.
    }

} // end of namespace
