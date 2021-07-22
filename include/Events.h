/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include "Mouse.h"
#include "Keyboard.h"
#include "SignalService.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// Mouse Events
    /////////////////////////////////////////////////

    /**
     * This event represents a mouse button event.
     * 
     * This event is supposed to be used in combination with the SignalService.
     * 
     * @ingroup input_group
     */
    class MouseButtonEvent {
    public:

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
        virtual bool OnMouseButtonPressed(int button, int x, int y) { 
            return false;
        }

        /** 
         * Called by this base class when a key has been released.
         * 
         * @param button    the button which has been released
         * @param x         the x-coordinate of the mouse course in screen space
         * @param y         the y-coordinate of the mouse course in screen space
         */
        virtual bool OnMouseButtonReleased(int button, int x, int y) { 
            return false;
        }

    private:

        // Inherited via MouseButtonListener 
        virtual bool OnSignal(const MouseButtonEvent & signal) {
            if (signal.pressed) {
                return OnMouseButtonPressed(signal.button, signal.x, signal.y);
            } else {
                return OnMouseButtonReleased(signal.button, signal.x, signal.y);
            }
        }
    };

    /**
     * This event represents a mouse wheel event.
     * 
     * This event is supposed to be used in combination with the SignalService.
     * 
     * @ingroup input_group
     */
    class MouseWheelEvent {
    public:

        /**
         * Constructor.
         *
         * @param amount the amount scrolled 
         */
        MouseWheelEvent(int amount) : amount(amount) {}

        int amount;
    };

    /** 
     * Type definition for signal services used to transmit mouse wheel events.
     *
     * @ingroup input_group
     */
    using MouseWheelEventService = SignalService<MouseWheelEvent>;

    /** 
     * Type definition for signal listeners which receive mouse wheel events.
     *
     * @ingroup input_group
     */
    using IMouseWheelListener = ISignalListener<MouseWheelEvent>;

    /**
     * Services can derive from this class to process mouse wheel events.
     */
    class MouseWheelListener : virtual public Service, private IMouseWheelListener {
    public:

        /**
         * Constructor.
         */
        MouseWheelListener() {
            AddStartupHook([this](){ 
                    ASTU_SERVICE(MouseWheelEventService).AddListener(*this); 
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(MouseWheelEventService).RemoveListener(*this); 
            });
        }

    protected:

        /** 
         * Called by this base class when a mouse wheel event has occurred.
         * 
         * @param amount    the amount scrolled 
         */
        virtual bool OnMouseWheel(int amount) { 
            return false;
        }

    private:

        // Inherited via MouseButtonListener 
        virtual bool OnSignal(const MouseWheelEvent & signal) {
            return OnMouseWheel(signal.amount);
        }
    };

    /**
     * This event represents a mouse movement event.
     * 
     * This event is supposed to be used in combination with the SignalService.
     * 
     * @ingroup input_group
     */
    class MouseMoveEvent {
    public:

        /**
         * Constructor.
         * 
         * @param x the x-coordinate of the mouse cursor in screen coordinates
         * @param x the y-coordinate of the mouse cursor in screen coordinates
         */
        MouseMoveEvent(int x = 0, int y = 0)
            : x(x), y(y) {}

        /** The x-coordinate of the mouse cursor. */
        int x;

        /** The y-coordinate of the mouse cursor. */
        int y;
    };

    /** 
     * Type definition for signal services used to transmit mouse wheel events.
     *
     * @ingroup input_group
     */
    using MouseMoveEventService = SignalService<MouseMoveEvent>;

    /** 
     * Type definition for signal listeners which receive mouse wheel events.
     *
     * @ingroup input_group
     */
    using IMouseMoveListener = ISignalListener<MouseMoveEvent>;


    /**
     * Services can derive from this class to process mouse wheel events.
     */
    class MouseMoveListener : virtual public Service, private IMouseMoveListener {
    public:

        /**
         * Constructor.
         */
        MouseMoveListener() {
            AddStartupHook([this](){ 
                    ASTU_SERVICE(MouseMoveEventService).AddListener(*this); 
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(MouseMoveEventService).RemoveListener(*this); 
            });
        }

    protected:

        /** 
         * Called by this base class when a mouse wheel event has occurred.
         * 
         * @param x the x-coordinate of the mouse cursor in screen coordinates
         * @param x the y-coordinate of the mouse cursor in screen coordinates
         */
        virtual bool OnMouseMove(int x, int y) { 
            return false;
        }

    private:

        // Inherited via MouseButtonListener 
        virtual bool OnSignal(const MouseMoveEvent & signal) {
            return OnMouseMove(signal.x, signal.y);
        }
    };


    /////////////////////////////////////////////////
    /////// Key Events
    /////////////////////////////////////////////////

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
            AddStartupHook([this](){ 
                ASTU_SERVICE(KeystrokeEventService).AddListener(*this);
            });

            AddShutdownHook([this](){ 
                ASTU_SERVICE(KeystrokeEventService).RemoveListener(*this);
            });
        }

    protected:

        /** 
         * Called by this base class when a key has been pressed.
         * 
         * @param keycode   the code of the key
         */
        virtual bool OnKeyPressed(int keycode) { return false; }

        /** 
         * Called by this base class when a key has been released.
         * 
         * @param keycode   the code of the key
         */
        virtual bool OnKeyReleased(int keycode) { return false; }

    private:

        // Inherited via KeystrokeListener
        virtual bool OnSignal(const KeystrokeEvent & signal) override {
            if (signal.pressed) {
                return OnKeyPressed(signal.keycode);
            } else {
                return OnKeyReleased(signal.keycode);
            }
        }

    };    

    /////////////////////////////////////////////////
    /////// Window Events
    /////////////////////////////////////////////////

    /**
     * This event represents a change in window size
     * 
     * This event is supposed to be used in combination with the SignalService.
     */
    class ResizeEvent {
    public:

        ResizeEvent(int width, int height)
            : width(width), height(height) {}

        /** The current width of the application window. */
        int width;

        /** The current width of the application window. */
        int height;
    };

    /** 
     * Type definition for signal listeners which receive resize events.
     */
    using IResizeListener = ISignalListener<ResizeEvent>;

    /** 
     * Type definition for signal services used to transmit resize events.
     *
     * @ingroup input_group
     */
    using ResizeEventService = SignalService<ResizeEvent>;

    /**
     * Services can derive from this class to process window resize events.
     */
    class ResizeListener : virtual public Service, private IResizeListener {
    public:

        /**
         * Constructor.
         */
        ResizeListener() {
            AddStartupHook([this](){ ASTU_SERVICE(ResizeEventService).AddListener(*this); });
            AddShutdownHook([this](){ ASTU_SERVICE(ResizeEventService).RemoveListener(*this); });
        }

    protected:

        /** 
         * Called by this base class when a resize event has occurred.
         * 
         * @param width the new window width
         * @param height the new window height
         */
        virtual bool OnResize(int width, int height) { return false; }

    private:

        // Inherited via KeystrokeListener
        virtual bool OnSignal(const ResizeEvent & signal) override {
           return OnResize(signal.width, signal.height);
        }
    };

} // end of namespace