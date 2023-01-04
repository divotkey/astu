/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include <Graphics/Texture.h>

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>

namespace astu {

    class SdlTexture : public Texture {
    public:

        /**
         * Constructor.
         *
         * @param sdlTexture    the SDL texture wrapped by this class
         */
        SdlTexture(SDL_Texture *sdlTexture);

        /**
         * Constructor.
         *
         * @param sdlTexture    the SDL texture wrapped by this class
         * @param width         the width of this texture in pixels
         * @param height        the height of this texture in pixels
         */
        SdlTexture(SDL_Texture *sdlTexture, int width, int height);

        /**
         * Virtual destructor.
         */
        virtual ~SdlTexture();

        /**
         * Returns the underlying SDL texture.
         *
         * @return the SDL texture
         */
        SDL_Texture *GetSdlTexture();

        /**
         * Returns the underlying SDL texture.
         *
         * @return the SDL texture
         */
        const SDL_Texture *GetSdlTexture() const;

        /**
         * Implicit conversion to raw SDL_Texture pointer.
         *
         * @return the underlying SDL texture
         */
        operator SDL_Texture *() { return sdlTexture; }

        // Inherited via Texture
        int GetWidth() const override;

        int GetHeight() const override;

        void SetBlendMode(BlendMode mode) override;

        BlendMode GetBlendMode() const override;

    private:
        /** The underlying SDL texture. */
        SDL_Texture *sdlTexture;

        /** The width of this texture in pixels. */
        int width;

        /** The height of this texture in pixels. */
        int height;

        /** The blend mode of this texture. */
        BlendMode mode;

        void TransferMode();
    };

} // end of namespace



