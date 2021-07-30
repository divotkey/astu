/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Util/MessageBox.h"

// SDL includes
#include <SDL2/SDL.h>

using namespace std;

namespace astu {

    void MessageBox::ShowErrorMessage(const string& message, const string& title)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), message.c_str(), nullptr);
    }

} // end of namespace