/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "SignalService.h"

namespace astu {

    class MouseButtonEvent {
    public:
        enum BUTTON {LEFT = 1, MIDDLE = 2, RIGHT = 3};

        MouseButtonEvent(int _button = 0, bool _pressed = false)
            : button(_button), pressed(_pressed) {}

        int button;
        bool pressed;
    };

    using MouseButtonEventService = SignalService<MouseButtonEvent>;
    using MouseButtonListener = ISignalListener<MouseButtonEvent>;
};