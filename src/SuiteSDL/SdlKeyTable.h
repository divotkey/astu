/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Input/InputMappingService.h"

// SDL 2 includes
#include <SDL2/SDL.h>

// C++ Standard Library includes
#include <array>

namespace astu {

    class SdlKeyTable {
    public:
        static const Key& ScanCodeToKey(int scancode);

    private:
        /** Maps SDL scancodes to astu keys. */
        static std::array<Key, SDL_Scancode::SDL_NUM_SCANCODES> table;
    };

} // end of namespace
