/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Input/Mouse.h"
#include "Suite2D/CameraService.h"

// C++ Standard Library includes
#include <string>
#include <stdexcept>

namespace astu {

    std::vector<bool> Mouse::buttons;
    int Mouse::cursorX = 0;
    int Mouse::cursorY = 0;

    void Mouse::SetButton(int button, bool pressed)
    {
        if (button < 0) {
            throw std::logic_error("Invalid mouse button index " 
                + std::to_string(button));
        }

        if (button >= static_cast<int>(buttons.size())) {
            buttons.resize(button + 1);
        }
        buttons[button] = pressed;
    }

    bool Mouse::IsPressed(int button) const
    {
        if (button < 0 || static_cast<int>(button >= buttons.size())) {
            return false;
        }

        return buttons[button];
    }

    void Mouse::SetCursor(int x, int y)
    {
        cursorX = x;
        cursorY = y;
    }

    int Mouse::GetCursorX() const
    {
        return cursorX;
    }

    int Mouse::GetCursorY() const
    {
        return cursorY;
    }

    Vector2f Mouse::GetCursorInWorldspace(const suite2d::Camera& camera)
    {
        return camera.GetInverseMatrix()
        .TransformPoint(
            static_cast<float>(cursorX), 
            static_cast<float>(cursorY));
    }


} // end of namespace