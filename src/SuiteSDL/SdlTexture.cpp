/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SdlTexture.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>

namespace astu {

    SdlTexture::SdlTexture(SDL_Texture *sdlTexture, int width, int height)
        : sdlTexture(sdlTexture), width(width), height(height)
    {
        // Intentionally left empty.
    }

    SdlTexture::SdlTexture(SDL_Texture *sdlTexture)
            : sdlTexture(sdlTexture)
    {
        if (SDL_QueryTexture(sdlTexture, NULL, NULL, &width, &height)) {
            throw std::runtime_error(std::string("Unable to query SDL texture: ") + SDL_GetError());
        }
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

}

