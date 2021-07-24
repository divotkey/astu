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
#include "SuiteSDL/SdlSceneRenderer2D.h"

// Forward declaration.
struct SDL_Renderer;

namespace astu {

    class SdlRecordingSceneRenderer2D : public SdlSceneRenderer2D {
    public:

        /**
         * Constructor.
         */
        SdlRecordingSceneRenderer2D();

        // Inherited via Scene2Renderer
        virtual void Render(suite2d::Polyline& polyline, float alpha) override;

    private:
    };

} // end of namespace