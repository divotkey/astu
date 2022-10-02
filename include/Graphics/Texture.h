/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>
#include <memory>

namespace astu {

    // Forward declaration
    class Image;

    /**
     * Abstraction layer class for (in most cases) hardware accelerated textures used for 2D games etc.
     *
     * @ingroup gfx_group
     */
    class Texture {
    public:

        /** Virtual destructor. */
        virtual ~Texture() {}

        /**
         * Returns the width of this texture in pixels.
         *
         * @return the width in pixels
         */
        virtual int GetWidth() const = 0;

        /**
         * Returns the height of this texture in pixels.
         *
         * @return the height in pixels
         */
        virtual int GetHeight() const = 0;

    };


    /**
     * Abstract base class to created implementation dependent textures.
     *
     * @ingroup gfx_group
     */
    class TextureFactory {
    public:

        /** Virtual destructor. */
        virtual ~TextureFactory() {}

        /**
         * Creates a new texture object based on the content of the specified image.
         *
         * @param image the image which size and content is used to create the texture
         * @return the newly created texture
         */
        virtual std::shared_ptr<Texture> CreateFromImage(const Image &image) const = 0;

        /**
         * Creates a new texture object based on the content of the specified Windows .BMP file.
         *
         * @param filepath  the path to the BMP file to use as texture content
         * @return the newly created texture
         */
        virtual std::shared_ptr<Texture> CreateFromBmp(const std::string& filepath) const = 0;
    };

} // end of namespace