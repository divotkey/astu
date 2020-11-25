/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

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

        MouseButtonEvent(int _button = 0, bool _pressed = false)
            : button(_button), pressed(_pressed) {}

        int button;
        bool pressed;
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
    using MouseButtonListener = ISignalListener<MouseButtonEvent>;

} // end of namespace