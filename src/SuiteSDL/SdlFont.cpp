/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SdlFont.h"
#include "SdlTexture.h"

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>
#include <string>

using namespace std;

namespace astu {

    SdlFont::SdlFont(TTF_Font *sdlFont, SDL_Renderer *renderer) : sdlFont(sdlFont), renderer(renderer)
    {
        assert(sdlFont);
    }

    TTF_Font *SdlFont::GetSdlFont()
    {
        return sdlFont;
    }

    const TTF_Font *SdlFont::GetSdlTexture() const
    {
        return sdlFont;
    }

    std::shared_ptr<Texture> SdlFont::GenerateTexture(const string &text, const Color4f &c)
    {
        assert(sdlFont);
        assert(renderer);

        SDL_Color sdlColor = {static_cast<Uint8>(c.r * 255),
                              static_cast<Uint8>(c.g * 255),
                              static_cast<Uint8>(c.b * 255),
                              static_cast<Uint8>(c.a * 255)};

        SDL_Surface *surface = TTF_RenderUTF8_Blended(sdlFont, text.c_str(), sdlColor);
        if (!surface) {
            throw std::runtime_error(string("Unable to generate surface, while rendering font: ") + SDL_GetError());
        }

        SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!sdlTexture) {
            SDL_FreeSurface(surface);
            throw std::runtime_error(
                    string("Unable to create SDL texture from surface while rendering font: ") + SDL_GetError());
        }
        SDL_FreeSurface(surface);

        return make_shared<SdlTexture>(sdlTexture);
    }

    std::shared_ptr<Texture> SdlFont::GenerateTexture(const wstring &text, const Color4f &c)
    {
        assert(sdlFont);
        assert(renderer);

        SDL_Color sdlColor = {static_cast<Uint8>(c.r * 255),
                              static_cast<Uint8>(c.g * 255),
                              static_cast<Uint8>(c.b * 255),
                              static_cast<Uint8>(c.a * 255)};

        const wchar_t *wText = text.data();

        SDL_Surface *surface = TTF_RenderUNICODE_Blended(sdlFont, reinterpret_cast<const uint16_t*>(wText), sdlColor);
        //SDL_Surface *surface = TTF_RenderText_Solid(sdlFont, text.c_str(), sdlColor);
        if (!surface) {
            throw std::runtime_error(string("Unable to generate surface, while rendering font: ") + SDL_GetError());
        }

        SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!sdlTexture) {
            SDL_FreeSurface(surface);
            throw std::runtime_error(
                    string("Unable to create SDL texture from surface while rendering font: ") + SDL_GetError());
        }
        SDL_FreeSurface(surface);

        return make_shared<SdlTexture>(sdlTexture);
    }

}

