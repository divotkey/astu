/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "AstUtilsConfig.h"
#include "Util/MessageBox.h"

using namespace std;

#ifdef ASTU_USE_SDL
// SDL includes
#include <SDL2/SDL.h>

namespace astu {

    void MessageBox::ShowErrorMessage(const string& message, const string& title)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), message.c_str(), nullptr);
    }

} // end of namespace

#else
// C++ Standard Library includes.
#include <iostream>

namespace astu {

    void MessageBox::ShowErrorMessage(const std::string& message, const std::string& title)
    {
        std::cout << message << std::endl;
    }

} // end of namespace

#endif