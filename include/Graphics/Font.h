/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include <Graphics/Color.h>
#include "Graphics/Texture.h"


// C++ Standard Library includes
#include <string>
#include <memory>

namespace astu {

    /**
     * Abstraction layer class for fonts used for text rendering.
     *
     * @ingroup gfx_group
     */
    class Font {
    public:

        /** Virtual destructor. */
        virtual ~Font() {}

        virtual std::shared_ptr<Texture> GenerateTexture(const std::string& text, const Color4f &color) = 0;
        virtual std::shared_ptr<Texture> GenerateTexture(const std::wstring& text, const Color4f &color) = 0;

    };

    /**
     * Abstract base class to created implementation dependent fonts.
     *
     * @ingroup gfx_group
     */
    class FontFactory {
    public:

        /** Virtual destructor. */
        virtual ~FontFactory() {}

        /**
         * Creates a new texture object based on the content of the specified image.
         *
         * @param fontFile  path to the font file, e.g. "Sans.ttf"
         * @param pointSize point size to use for the newly-opened font
         * @return the newly created font
         */
        virtual std::shared_ptr<Font> LoadFont(const std::string &fontFile, int pointSize) const = 0;

    };


} // end of namespace