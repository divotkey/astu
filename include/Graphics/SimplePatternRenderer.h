/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/PatternRenderer.h"

namespace astu {

    /**
     * A straight forward implementation of an pattern renderer, without anti-aliasing.
     */
    class SimplePatternRenderer : public PatternRenderer {
    public:

        /**
         * Constructor.
         *
         * @param mode  the transform mode
         */
        SimplePatternRenderer(TransformMode mode = TransformMode::RAW) : PatternRenderer(mode) {}

    protected:
        // Inherited via SimplePatternRenderer
        virtual void DoRender(const Pattern &pattern, Image &result) override;
    };

} // end of namespace