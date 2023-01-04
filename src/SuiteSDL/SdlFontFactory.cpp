/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SdlFontFactory.h"
#include "SdlFont.h"
#include "SuiteSDL/SdlRenderService.h"

// Simple Direct Layer (SDL) includes
#include <SDL2/SDL_ttf.h>

using namespace std;

namespace astu {

    SdlFontFactory::SdlFontFactory()
        : Service("SDL Font Factory Service")
    {
        // Intentionally left empty.
    }

    std::shared_ptr<Font> SdlFontFactory::LoadFont(const std::string &fontFile, int pointSize) const
    {
        TTF_Font *font = TTF_OpenFont(fontFile.c_str(), pointSize);
        if (!font) {
            throw std::logic_error(string("Unable to load font file '") + fontFile + "': " + SDL_GetError());
        }

        return make_shared<SdlFont>(font, ASTU_SERVICE(SdlRenderService).GetRenderer());
    }

    void SdlFontFactory::OnStartup()
    {
        if (TTF_Init()) {
            throw std::runtime_error(string("Unable to initialize SDL2 TTL Library") + SDL_GetError());
        }
    }

    void SdlFontFactory::OnShutdown()
    {
        TTF_Quit();
    }

} // end of namespace