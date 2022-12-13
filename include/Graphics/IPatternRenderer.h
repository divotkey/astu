/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Graphics/Color.h"
#include "Graphics/RenderQuality.h"
#include "Velox/Interpreter/ItemData.h"

// C++ Standard Library includes
#include <map>

namespace astu {

    // Forward declaration
    class Pattern;
    class Image;

    /**
     * Interface defining for rasterizer patterns to an image.
     *
     * Note: deriving interface class from ItemData makes integration into Velox scripting language
     * way more easier.
     */
    class IPatternRenderer : public velox::ItemData {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~IPatternRenderer() {}

        virtual void Render(const Pattern &pattern, Image &result) = 0;
    };

} // end of namespace