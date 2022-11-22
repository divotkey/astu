/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SdlTexture.h"

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>
#include <string>

using namespace std;

namespace astu {

    SdlTexture::SdlTexture(SDL_Texture *sdlTexture, int width, int height)
            : sdlTexture(sdlTexture), width(width), height(height), mode(BlendMode::Blend)
    {
        assert(sdlTexture);
        TransferMode();
    }

    SdlTexture::SdlTexture(SDL_Texture *sdlTexture)
            : sdlTexture(sdlTexture), mode(BlendMode::Blend)
    {
        assert(sdlTexture);
        if (SDL_QueryTexture(sdlTexture, NULL, NULL, &width, &height)) {
            throw std::runtime_error(std::string("Unable to query SDL texture: ") + SDL_GetError());
        }

        TransferMode();
    }

    SdlTexture::~SdlTexture()
    {
        if (sdlTexture) {
            SDL_DestroyTexture(sdlTexture);
        }
    }

    SDL_Texture *SdlTexture::GetSdlTexture()
    {
        return sdlTexture;
    }

    const SDL_Texture *SdlTexture::GetSdlTexture() const
    {
        return sdlTexture;
    }

    int SdlTexture::GetWidth() const
    {
        return width;
    }

    int SdlTexture::GetHeight() const
    {
        return height;
    }

    void SdlTexture::SetBlendMode(Texture::BlendMode inMode)
    {
        if (mode != inMode) {
            mode = inMode;
            TransferMode();
        }
    }

    Texture::BlendMode SdlTexture::GetBlendMode() const
    {
        return mode;
    }

    void SdlTexture::TransferMode()
    {
        assert(sdlTexture);

        SDL_BlendMode sdlBlendMode;
        switch (mode) {

            case BlendMode::None:
                sdlBlendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
                break;

            case BlendMode::Blend:
                sdlBlendMode = SDL_BlendMode::SDL_BLENDMODE_BLEND;
                break;

            case BlendMode::Add:
                sdlBlendMode = SDL_BlendMode::SDL_BLENDMODE_ADD;
                break;

            case BlendMode::Mod:
                sdlBlendMode = SDL_BlendMode::SDL_BLENDMODE_MOD;
                break;

            default:
                throw std::logic_error("Unknown blend mode");
        }

        if (SDL_SetTextureBlendMode(sdlTexture, sdlBlendMode)) {
            throw std::runtime_error(string("Unable to set SDL blend mode: ")
                                     + to_string(sdlBlendMode) + ": " + SDL_GetError());
        }

    }

}

