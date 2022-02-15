/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SdlKeyTable.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace astu {

    array<Key, SDL_Scancode::SDL_NUM_SCANCODES> SdlKeyTable::table = {
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("A"), Key("B"), Key("C"), Key("D"),
            Key("E"), Key("F"), Key("G"), Key("H"),
            Key("I"), Key("J"), Key("K"), Key("L"),
            Key("M"), Key("N"), Key("O"), Key("P"),
            Key("Q"), Key("R"), Key("S"), Key("T"),
            Key("U"), Key("V"), Key("W"), Key("X"),
            Key("Y"), Key("Z"), Key("One"), Key("Two"),
            Key("Three"), Key("Four"), Key("Five"), Key("Six"),
            Key("Seven"), Key("Eight"), Key("Nine"), Key("Zero"),
            Key("Return"), Key("Escape"), Key("Backspace"), Key("Tab"),
            Key("SpaceBar"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("F1"), Key("F2"),
            Key("F3"), Key("F4"), Key("F5"), Key("F6"),
            Key("F7"), Key("F8"), Key("F9"), Key("F10"),
            Key("F11"), Key("F12"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("Home"), Key("PageUp"),
            Key("Delete"), Key("End"), Key("PageDown"), Key("Right"),
            Key("Left"), Key("Down"), Key("Up"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("NumpadOne"), Key("NumpadTwo"), Key("NumpadThree"),
            Key("NumpadFour"), Key("NumpadFive"), Key("NumpadSix"), Key("NumpadSeven"),
            Key("NumpadEight"), Key("NumpadNine"), Key("NumpadZero"), Key("NumpadPeriod"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("LeftControl"), Key("LeftShift"), Key("LeftAlt"), Key("LeftGui"),
            Key("RightControl"), Key("RightShift"), Key("RightAlt"), Key("RightGui"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"), Key("UNKNOWN"),
            Key("UNKNOWN")
    };

    const Key& SdlKeyTable::ScanCodeToKey(int scancode)
    {
        assert(scancode >= 0 && scancode <= SDL_Scancode::SDL_NUM_SCANCODES);
        return table.at(scancode);
    }

} // end of namespace
