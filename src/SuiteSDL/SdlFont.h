/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include <Graphics/Font.h>

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace astu {

    class SdlFont : public Font {
    public:

        /**
         * Constructor.
         *
         * @param sdlFont   the SDL font wrapped by this class
         * @param renderer  the renderer used to generate textures
         */
        SdlFont(TTF_Font *sdlFont, SDL_Renderer *renderer);

        /**
         * Virtual destructor.
         */
        virtual ~SdlFont() {}

        /**
         * Returns the underlying SDL font.
         *
         * @return the SDL font
         */
        TTF_Font *GetSdlFont();

        /**
         * Returns the underlying SDL font.
         *
         * @return the SDL font
         */
        const TTF_Font *GetSdlTexture() const;

        /**
         * Implicit conversion to raw TTF_Font pointer.
         *
         * @return the underlying SDL texture
         */
        operator TTF_Font *() { return sdlFont; }

        // Inherited via Font
        std::shared_ptr<Texture> GenerateTexture(const std::string& text, const Color4f &color) override;
        virtual std::shared_ptr<Texture> GenerateTexture(const std::wstring& text, const Color4f &color) override;

    private:
        /** The underlying SDL texture. */
        TTF_Font *sdlFont;

        /** Renderer used to generate textures. */
        SDL_Renderer *renderer;
    };

} // end of namespace



