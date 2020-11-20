/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <string>
#include <stdexcept>
#include "Mouse.h"

namespace astu {

    std::vector<bool> Mouse::buttons;

    void Mouse::SetButton(int button, bool pressed)
    {
        if (button < 0) {
            throw std::logic_error("Invalid mouse button index " 
                + std::to_string(button));
        }

        if (button >= buttons.size()) {
            buttons.resize(button + 1);
        }
        buttons[button] = pressed;
    }

    bool Mouse::IsPressed(int button) const
    {
        if (button < 0 || button >= buttons.size()) {
            return false;
        }

        return buttons[button];
    }


} // end of namespace