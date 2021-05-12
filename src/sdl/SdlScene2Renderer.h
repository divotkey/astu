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

namespace astu {

    class SdlScene2Renderer : public Scene2Renderer {
    public:

        // Inherited via Scene2Renderer
        virtual void Render(Polyline2& polyline) override;
    };

} // end of namespace