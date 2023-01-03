/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Input/Keys.h"

namespace astu {

    class ISdlApplication1Listener {
    public:

        /** Virtual destructor. */
        virtual ~ISdlApplication1Listener() {}

        /**
         * Called render application specific content.
         */
        virtual void OnRender() {}

        /**
         * Called, if a key down event occurs.
         *
         * @param key   the key which has been pressed
         */
        virtual void OnKeyDown(const astu::Key &key) = 0;

        /**
         * Called, if a key up event occurs.
         *
         * @param key   the key which has been pressed
         */
        virtual void OnKeyUp(const astu::Key &key) = 0;

        /**
         * Called, if a mouse button has been pressed.
         *
         * @param button    the index of the mouse button
         * @param x         the x-coordinate of the cursor in screen space
         * @param y         the y-coordinate of the cursor in screen space
         */
        virtual void OnMouseButtonDown(int button, int x, int y) = 0;

        /**
         * Called, if a mouse button has been released.
         *
         * @param button    the index of the mouse button
         * @param x         the x-coordinate of the cursor in screen space
         * @param y         the y-coordinate of the cursor in screen space
         */
        virtual void OnMouseButtonUp(int button, int x, int y) = 0;

    };

} // End of namespace