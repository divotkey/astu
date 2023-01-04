/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
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
     * Base class for rasterizer that render patterns to an image.
     *
     * Note: deriving interface class from ItemData makes integration into Velox scripting language
     * way more easier.
     */
    class PatternRenderer : public velox::ItemData {
    public:

        /** Defines the transformation used to render patterns to images. */
        enum class TransformMode {

            /** The patterns will not be transformed, patterns coordinates are image coordinates. */
            RAW,

            /** No scaling but content will be centered in the middle of the image. */
            CENTERED,

            /** The horizontal image size will be set to 1.0, the vertical size according to the aspect ratio. */
            H_NORMALIZED,

            /** The vertical image size will be set to 1.0, the horizontal size according to the aspect ratio. */
            V_NORMALIZED,

            /** The vertical and horizontal size of the image will be set to 1.0. */
            NORMALIZED
        };

        /**
         * Constructor.
         *
         * @param mode  the transform mode
         */
        explicit PatternRenderer(TransformMode mode);

        /**
         * Virtual destructor.
         */
        virtual ~PatternRenderer() {}

        /**
         * Sets the transform mode, defining the transformation of patterns while rendering.
         *
         * @param mode
         */
        void SetTransformMode(TransformMode mode);

        /**
         * Returns the transform mode of this renderer.
         *
         * @return the transform mode
         */
        TransformMode GetTransformMode() const;

        /**
         * Renders a pattern to an image.
         *
         * @param pattern   the pattern to render
         * @param result    the image the rendered pattern will be added to
         */
        void Render(const Pattern &pattern, Image &result);

    protected:
        /** The transformation used to render patterns to images. */
        Matrix3d transform;

        /** The the actual implementation of this renderer. */
        virtual void DoRender(const Pattern &pattern, Image &result) = 0;

    private:
        /** The render mode. */
        TransformMode mode;

        /**
         * Calculates the transformation according to the current configuration.
         */
        void UpdateTransform(double imageWidth, double imageHeight);
    };

} // end of namespace