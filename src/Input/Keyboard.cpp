/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Input/Keyboard.h"

// C++ Standard Library includes
#include <string>
#include <stdexcept>

namespace astu {

    std::vector<bool> Keyboard::keys;

    void Keyboard::SetKey(int keycode, bool pressed)
    {
        if (keycode < 0) {
            throw std::logic_error("Invalid keycode " 
                + std::to_string(keycode));
        }

        if (keycode >= keys.size()) {
            keys.resize(keycode + 1, false);
        }

        keys[keycode] = pressed;
    }

    bool Keyboard::IsPressed(int keycode) const
    {
        if (keycode < 0 || keycode >= keys.size()) {
            return false;
        }

        return keys[keycode];
    }

} // end of namespace