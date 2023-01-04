/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Service/SignalService.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// Mouse Signals
    /////////////////////////////////////////////////

    /**
     * This signal represents a mouse button event.
     * 
     * @ingroup input_group
     */
    class MouseButtonSignal {
    public:

        /**
         * Constructor.
         * 
         * @param button    the button which has been pressed or released
         * @param pressed   `true` if the button has been pressed, `false` if
         *                  the button has been released
         * @param x         the x-coordinate of the mouse cursor
         * @param y         the y-coordinate of the mouse cursor
         */
        MouseButtonSignal(int button = 0, bool pressed = false, int x = 0, int y = 0)
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
     * Type definition for signal services that transmit mouse button signals.
     *
     * @ingroup input_group
     */
    using MouseButtonSignalService = SignalService<MouseButtonSignal>;

    /** 
     * Type definition for signal listeners which receive mouse button signals.
     *
     * @ingroup input_group
     */
    using IMouseButtonListener = ISignalListener<MouseButtonSignal>;

    /**
     * Services can derive from this class to process mouse button signals.
     * 
     * @ingroup input_group
     */
    class MouseButtonListener 
        : virtual public Service
        , private IMouseButtonListener
    {
    public:

        /**
         * Constructor.
         */
        MouseButtonListener() {
            AddStartupHook([this](){ 
                ASTU_SERVICE(MouseButtonSignalService).AddListener(*this); 
                });

            AddShutdownHook([this](){ 
                ASTU_SERVICE(MouseButtonSignalService).RemoveListener(*this); 
                });
        }

    protected:

        /** 
         * Called by this base class when a mouse button has been pressed.
         * 
         * @param button    the button which has been pressed
         * @param x         the x-coordinate of the mouse course in screen space
         * @param y         the y-coordinate of the mouse course in screen space
         */
        virtual bool OnMouseButtonPressed(int button, int x, int y) { 
            return false;
        }

        /** 
         * Called by this base class when a mouse button has been released.
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
        virtual bool OnSignal(const MouseButtonSignal & signal) {
            if (signal.pressed) {
                return OnMouseButtonPressed(signal.button, signal.x, signal.y);
            } else {
                return OnMouseButtonReleased(signal.button, signal.x, signal.y);
            }
        }
    };

    /**
     * This signal represents a mouse wheel event.
     * 
     * @ingroup input_group
     */
    class MouseWheelSignal {
    public:

        /**
         * Constructor.
         *
         * @param amount the amount scrolled 
         */
        MouseWheelSignal(int amount) : amount(amount) {}

        /** The amount the mouse-wheel has been moved. */
        int amount;
    };

    /** 
     * Type definition for signal services used to transmit mouse wheel signals.
     *
     * @ingroup input_group
     */
    using MouseWheelSignalService = SignalService<MouseWheelSignal>;

    /** 
     * Type definition for signal listeners which receive mouse wheel signals.
     *
     * @ingroup input_group
     */
    using IMouseWheelListener = ISignalListener<MouseWheelSignal>;

    /**
     * Services can derive from this class to process mouse wheel signals.
     * 
     * @ingroup input_group
     */
    class MouseWheelListener 
        : virtual public Service
        , private IMouseWheelListener
    {
    public:

        /**
         * Constructor.
         */
        MouseWheelListener() {
            AddStartupHook([this](){ 
                    ASTU_SERVICE(MouseWheelSignalService).AddListener(*this); 
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(MouseWheelSignalService).RemoveListener(*this); 
            });
        }

    protected:

        /** 
         * Called by this base class when a mouse wheel signals has received.
         * 
         * @param amount    the amount scrolled 
         */
        virtual bool OnMouseWheel(int amount) { 
            return false;
        }

    private:

        // Inherited via MouseButtonListener 
        virtual bool OnSignal(const MouseWheelSignal & signal) {
            return OnMouseWheel(signal.amount);
        }
    };

    /**
     * This signal represents a mouse movement event.
     *      
     * @ingroup input_group
     */
    class MouseMoveSignal {
    public:

        /**
         * Constructor.
         * 
         * @param x the x-coordinate of the mouse cursor in screen coordinates
         * @param y the y-coordinate of the mouse cursor in screen coordinates
         */
        MouseMoveSignal(int x = 0, int y = 0)
            : x(x), y(y) {}

        /** The x-coordinate of the mouse cursor. */
        int x;

        /** The y-coordinate of the mouse cursor. */
        int y;
    };

    /** 
     * Type definition for signal services that transmit mouse wheel signals.
     *
     * @ingroup input_group
     */
    using MouseMoveSignalService = SignalService<MouseMoveSignal>;

    /** 
     * Type definition for signal listeners which receive mouse wheel signals.
     *
     * @ingroup input_group
     */
    using IMouseMoveListener = ISignalListener<MouseMoveSignal>;


    /**
     * Services can derive from this class to process mouse move signals.
     * 
     * @ingroup input_group
     */
    class MouseMoveListener 
        : virtual public Service
        , private IMouseMoveListener
    {
    public:

        /**
         * Constructor.
         */
        MouseMoveListener() {
            AddStartupHook([this](){ 
                    ASTU_SERVICE(MouseMoveSignalService).AddListener(*this); 
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(MouseMoveSignalService).RemoveListener(*this); 
            });
        }

    protected:

        /** 
         * Called by this base class when a mouse move signals has received.
         * 
         * @param x the x-coordinate of the mouse cursor in screen coordinates
         * @param y the y-coordinate of the mouse cursor in screen coordinates
         */
        virtual bool OnMouseMove(int x, int y) { 
            return false;
        }

    private:

        // Inherited via MouseButtonListener 
        virtual bool OnSignal(const MouseMoveSignal & signal) {
            return OnMouseMove(signal.x, signal.y);
        }
    };

    /////////////////////////////////////////////////
    /////// Keyboard signals
    /////////////////////////////////////////////////

    /**
     * This signal represents a keystroke event.
     *  
     * @ingroup input_group
     */
    class KeystrokeSignal {
    public:

        /**
         * Constructor.
         * 
         * @param keycode   the keycode of the key
         * @param pressed   `true` if the key has been pressed, `false` if the 
         *                  key has been released
         */
        KeystrokeSignal(int keycode = 0, bool pressed = false)
            : keycode(keycode), pressed(pressed) {}

        /** The keycode of the event. */
        int keycode;

        /** Whether the key has been pressed or released. */
        bool pressed;
    };

    /** 
     * Type definition for signal services that transmit keystroke signals.
     *
     * @ingroup input_group
     */
    using KeystrokeSignalService = SignalService<KeystrokeSignal>;

    /** 
     * Type definition for signal listeners which receive keystroke signals.
     *
     * @ingroup input_group
     */
    using IKeystrokeListener = ISignalListener<KeystrokeSignal>;

    /**
     * Services can derive from this class to process key strokes.
     */
    class KeystrokeListener 
        : virtual public Service
        , private IKeystrokeListener
    {
    public:

        /**
         * Constructor.
         */
        KeystrokeListener() {
            AddStartupHook([this](){ 
                ASTU_SERVICE(KeystrokeSignalService).AddListener(*this);
            });

            AddShutdownHook([this](){ 
                ASTU_SERVICE(KeystrokeSignalService).RemoveListener(*this);
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
        virtual bool OnSignal(const KeystrokeSignal & signal) override {
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
     * This signal represents a change in window size
     *  
     * @ingroup input_group
     */
    class ResizeSignal {
    public:

        /**
         * Constructor.
         * 
         * @param width     the new window width
         * @param height    the new window height
         */
        ResizeSignal(int width, int height)
            : width(width), height(height) {}

        /** The current width of the application window. */
        int width;

        /** The current width of the application window. */
        int height;
    };

    /** 
     * Type definition for signal listeners which receive resize signals.
     * 
     * @ingroup input_group
     */
    using IResizeListener = ISignalListener<ResizeSignal>;

    /** 
     * Type definition for signal services that transmit resize signals.
     *
     * @ingroup input_group
     */
    using ResizeSignalService = SignalService<ResizeSignal>;

    /**
     * Services can derive from this class to process window resize signals.
     * 
     * @ingroup input_group
     */
    class ResizeListener : virtual public Service, private IResizeListener {
    public:

        /**
         * Constructor.
         */
        ResizeListener() {
            AddStartupHook([this](){ 
                ASTU_SERVICE(ResizeSignalService).AddListener(*this); 
            });

            AddShutdownHook([this](){ 
                ASTU_SERVICE(ResizeSignalService).RemoveListener(*this);
            });
        }

    protected:

        /** 
         * Called by this base class when a resize signal has received.
         * 
         * @param width the new window width
         * @param height the new window height
         */
        virtual bool OnResize(int width, int height) { 
            return false;
        }

    private:

        // Inherited via KeystrokeListener
        virtual bool OnSignal(const ResizeSignal & signal) override {
           return OnResize(signal.width, signal.height);
        }
    };

    /**
     * Window signals are generated if the state of the window changes.
     * 
     * Window state changes are e.g., closing, minimizing of maximizing of the
     * application window.
     * 
     * @ingroup input_group
     */
    class WindowState {
    public:
    
        /** The possible types of this window signal. */
        enum class Type {CLOSE, MINIMIZED, MAXIMIZED, SHOWN};

        /** The type if this window signal. */
        Type type;

        /**
         * Constructor.
         */
        WindowState(Type type) : type(type) {}

    };    

    /** 
     * Type definition for signal listeners which receive window signals.
     * 
     * @ingroup input_group
     */
    using IWindowStateListener = ISignalListener<WindowState>;

    /** 
     * Type definition for signal services used to transmit window signals.
     *
     * @ingroup input_group
     */
    using WindowStateSignalService = SignalService<WindowState>;

    /////////////////////////////////////////////////
    /////// Drag and Drop
    /////////////////////////////////////////////////

    /**
     * This signal represents a drag and drop operation.
     *  
     * @ingroup input_group
     */
    class DropSignal {
    public:

        /** The types of dropped content. */
        enum Type {File, Text};

        /**
         * Constructor.
         * 
         * @param t     the type of the dropped content
         * @param cnt   the content of the drop signal
         */
        DropSignal(Type t, const std::string& cnt)
            : type(t), content(cnt) {}

        /** The type of the dropped content. */
        Type type;

        /** The file or text of the drop signal. */
        std::string content;

    };

    /** 
     * Type definition for signal listeners which receive drop signals.
     * 
     * @ingroup input_group
     */
    using IDropListener = ISignalListener<DropSignal>;

    /** 
     * Type definition for signal services used to transmit drop signals.
     *
     * @ingroup input_group
     */
    using DropSignalService = SignalService<DropSignal>;

    /**
     * Services can derive from this class to process mouse button signals.
     *
     * @ingroup input_group
     */
    class DropListener
            : virtual public Service
            , private IDropListener
    {
    public:

        /**
         * Constructor.
         */
        DropListener() {
            AddStartupHook([this](){
                ASTU_SERVICE(DropSignalService).AddListener(*this);
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(DropSignalService).RemoveListener(*this);
            });
        }

    protected:

        /**
         * Called by this base class when a file has been dropped on the application window
         *
         * @param filename  the path of the file that has been dropped
         */
        virtual bool OnFileDropped(const std::string &filename) {
            return false;
        }

        /**
         * Called by this base class when a text has been dropped on the application window
         *
         * @param text  the text that has been dropped
         */
        virtual bool OnTextDropped(const std::string &text) {
            return false;
        }

    private:

        // Inherited via MouseButtonListener
        virtual bool OnSignal(const DropSignal &signal) {
            switch (signal.type) {
                case DropSignal::Type::File:
                    return OnFileDropped(signal.content);

                case DropSignal::Type::Text:
                    return OnTextDropped(signal.content);

                default:
                    // should never happen.
                    return false;
            }
        }

    };


} // end of namespace