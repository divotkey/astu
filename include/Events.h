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
     * Services can derive from this class to process key strokes.
     */
    class MouseButtonListener : virtual public Service, private IMouseButtonListener {
    public:

        /**
         * Constructor.
         */
        MouseButtonListener() {
            AddStartupHook([this](){ ASTU_SERVICE(MouseButtonEventService).AddListener(*this); });
            AddShutdownHook([this](){ ASTU_SERVICE(MouseButtonEventService).RemoveListener(*this); });
        }

    protected:

        /** 
         * Called by this base class when a key has been pressed.
         * 
         * @param button    the button which has been pressed
         * @param x         the x-coordinate of the mouse course in screen space
         * @param y         the y-coordinate of the mouse course in screen space
         */
        virtual void OnMouseButtonPressed(int button, int x, int y) { }

        /** 
         * Called by this base class when a key has been released.
         * 
         * @param button    the button which has been released
         * @param x         the x-coordinate of the mouse course in screen space
         * @param y         the y-coordinate of the mouse course in screen space
         */
        virtual void OnMouseButtonReleased(int button, int x, int y) { }

    private:

        // Inherited via MouseButtonListener 
        virtual void OnSignal(const MouseButtonEvent & signal) {
            if (signal.pressed) {
                OnMouseButtonPressed(signal.button, signal.x, signal.y);
            } else {
                OnMouseButtonReleased(signal.button, signal.x, signal.y);
            }
        }
    };

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

    /**
     * Services can derive from this class to process key strokes.
     */
    class KeystrokeListener : virtual public Service, private IKeystrokeListener {
    public:

        /**
         * Constructor.
         */
        KeystrokeListener() {
            AddStartupHook([this](){ ASTU_SERVICE(KeystrokeEventService).AddListener(*this); });
            AddShutdownHook([this](){ ASTU_SERVICE(KeystrokeEventService).RemoveListener(*this); });
        }

    protected:

        /** 
         * Called by this base class when a key has been pressed.
         * 
         * @param keycode   the code of the key
         */
        virtual void OnKeyPressed(int keycode) {}

        /** 
         * Called by this base class when a key has been released.
         * 
         * @param keycode   the code of the key
         */
        virtual void OnKeyReleased(int keycode) {}

    private:

        // Inherited via KeystrokeListener
        virtual void OnSignal(const KeystrokeEvent & signal) override {
            if (signal.pressed) {
                OnKeyPressed(signal.keycode);
            } else {
                OnKeyReleased(signal.keycode);
            }
        }

    };    

} // end of namespace