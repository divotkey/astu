/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Simple Direct Layer (SDL) includes
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
