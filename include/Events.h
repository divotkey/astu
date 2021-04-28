/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include "SignalService.h"

namespace astu {

    /**
     * This event represents a mouse button event.
     * 
     * This event is supposed to be used in combination with the SignalService.
     * 
     * @ingroup input_group
     */
    class MouseButtonEvent {
    public:
        enum BUTTON {LEFT = 1, MIDDLE = 2, RIGHT = 3};

        MouseButtonEvent(int button = 0, bool pressed = false, int x = 0, int y = 0)
            : button(button), pressed(pressed), x(x), y(y) {}

        /** The button which has been pressed or released. */
        int button;

        /** Whether the button has been pressed or released. */
        bool pressed;

        /** The x-coordinate of the mouse cursor. */
        int x;

        /** The y-coordinate of the mouse cursor. */
        int y;
    };

    /** 
     * Type definition for signal services used to transmit mouse button events.
     *
     * @ingroup input_group
     */
    using MouseButtonEventService = SignalService<MouseButtonEvent>;

    /** 
     * Type definition for signal listeners which receive mouse button events.
     *
     * @ingroup input_group
     */
    using IMouseButtonListener = ISignalListener<MouseButtonEvent>;

    /**
     * This event represents a keystroke event.
     * 
     * This event is supposed to be used in combination with the SignalService.
     * 
     * @ingroup input_group
     */
    class KeystrokeEvent {
    public:

        KeystrokeEvent(int keycode = 0, bool pressed = false)
            : keycode(keycode), pressed(pressed) {}

        /** The keycode of the event. */
        int keycode;

        /** Whether the key has been pressed or released. */
        bool pressed;
    };

    /** 
     * Type definition for signal services used to transmit keystroke events.
     *
     * @ingroup input_group
     */
    using KeystrokeEventService = SignalService<KeystrokeEvent>;

    /** 
     * Type definition for signal listeners which receive keystroke events.
     *
     * @ingroup input_group
     */
    using IKeystrokeListener = ISignalListener<KeystrokeEvent>;

} // end of namespace