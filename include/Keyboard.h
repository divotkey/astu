/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <vector>

namespace astu {



    /**
     * Provides access to keyboard input.
     * 
     * This class is realized using the Monostate design pattern.
     * 
     * @ingroup input_group
     */
    class Keyboard {
    public:

        enum Keycodes {
            KEY_UNKNOWN = 0,
            KEY_A = 4,
            KEY_B = 5,
            KEY_C = 6,
            KEY_D = 7,
            KEY_E = 8,
            KEY_F = 9,
            KEY_G = 10,
            KEY_H = 11,
            KEY_I = 12,
            KEY_J = 13,
            KEY_K = 14,
            KEY_L = 15,
            KEY_M = 16,
            KEY_N = 17,
            KEY_O = 18,
            KEY_P = 19,
            KEY_Q = 20,
            KEY_R = 21,
            KEY_S = 22,
            KEY_T = 23,
            KEY_U = 24,
            KEY_V = 25,
            KEY_W = 26,
            KEY_X = 27,
            KEY_Y = 28,
            KEY_Z = 29,
            KEY_1 = 30,
            KEY_2 = 31,
            KEY_3 = 32,
            KEY_4 = 33,
            KEY_5 = 34,
            KEY_6 = 35,
            KEY_7 = 36,
            KEY_8 = 37,
            KEY_9 = 38,
            KEY_0 = 39,
            KEY_RETURN = 40,
            KEY_ESCAPE = 41,
            KEY_BACKSPACE = 42,
            KEY_TAB = 43,
            KEY_SPACE = 44,
            KEY_MINUS = 45,
            KEY_EQUALS = 46,
            KEY_RIGHT = 79,
            KEY_LEFT = 80,
            KEY_DOWN = 81,
            KEY_UP = 82,
            KEY_MAX = 48
            };

        /**
         * Sets the state of a key.
         * 
         * @param keycode    the keycode of the key to set
         * @param pressed   set to `true` to marked the key as pressed
         */
        void SetKey(int keycode, bool pressed);

        /**
         * Returns whether a key is currently pressed.
         * 
         * @param keycode    the keycode of the key to query
         * @return `true` if the key is currently pressed
         */
        bool IsPressed(int keycode) const;

    private:
        /** Represents the state of the mouse buttons. */
        static std::vector<bool> keys;
    };

} // end of namespace