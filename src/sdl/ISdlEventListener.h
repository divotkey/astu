/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// SDL 2 includes
#include <SDL2/SDL.h>

namespace astu {

    class ISdlEventListener {
    public:
        /** Virtual destructor. */
        virtual ~ISdlEventListener() {}

        /**
         * Called to handle an SDL event.
         * 
         * @param event the SDL event
         */
        virtual void HandleEvent(const SDL_Event & event) = 0;
    };

} // end of namespace
