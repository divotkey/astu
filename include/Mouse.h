/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <vector>

namespace astu {

    /**
     * Provides access to mouse input.
     * 
     * This class is realized using the Monostate design pattern.
     * 
     * @ingroup input_group
     */
    class Mouse {
    public:

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

    private:
        /** Represents the state of the mouse buttons. */
        static std::vector<bool> buttons;
    };

} // end of namespace