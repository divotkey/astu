/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include "Vector2.h"

// C++ Standard Library includes
#include <vector>

namespace astu {

    // Forward declaration.
    namespace suite2d {
        class Camera;
    }

    /**
     * Provides access to mouse input.
     * 
     * This class is realized using the Monostate design pattern.
     * 
     * @ingroup input_group
     */
    class Mouse {
    public:

        /** Enum constants for mouse buttons. */
        enum Button {LEFT = 1, MIDDLE = 2, RIGHT = 3};

        /**
         * Sets the state of a button.
         * 
         * @param button    the index of the button to set
         * @param pressed   set to `true` to marked the button as pressed
         */
        void SetButton(int button, bool pressed);

        /**
         * Returns whether a button is currently pressed.
         * 
         * @param button    the index of the button to query
         * @return `true` if the button is currently pressed
         */
        bool IsPressed(int button) const;

        /**
         * Sets the position of the mouse cursor.
         * 
         * @param x the x-coorindate of the mouse cursor
         * @param y the y-coorindate of the mouse cursor
         */
        void SetCursor(int x, int y);

        /**
         * Returns the x-coordinate of the mouse cursor.
         * 
         * @return the x-coordinate
         */
        int GetCursorX() const;

        /**
         * Returns the y-coordinate of the mouse cursor.
         * 
         * @return the y-coordinate
         */
        int GetCursorY() const;

        /**
         * Returns the cursor position in world space.
         * 
         * @param camera    the camera used to transform screen space coords
         * @return the cursor position in world space
         */
        Vector2f GetCursorInWorldspace(const suite2d::Camera& camera);

    private:
        /** Represents the state of the mouse buttons. */
        static std::vector<bool> buttons;

        /** The x-coordinate of the mouse position. */
        static int cursorX;

        /** The y-coordinate of the mouse position. */
        static int cursorY;
    };

} // end of namespace