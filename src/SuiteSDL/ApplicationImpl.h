/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// AST-Utilities includes
#include "Graphics/Color.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "SuiteSDL/ISdlApplication1Listener.h"

// SDL2 includes
#include <SDL2/SDL.h>

// C++ Standard Library includes
#include <string>

namespace astu {

    class ApplicationImpl {
    public:
        /** The default window width. */
        static const int kDefaultWidth = 800;

        /** The default window height. */
        static const int kDefaultHeight = 600;

        /** The default background color. */
        static const Color4d kDefaultBackgroundColor;

        /** The default draw color. */
        static const Color4d kDefaultDrawColor;

        /**
         * Constructor.
         */
        ApplicationImpl();

        void SetWidth(int w);
        void SetHeight(int h);
        int GetWidth() const;
        int GetHeight() const;    
        void SetBackgroundColor(const Color4d & c);
        Color4d GetBackgroundColor() const;
        void SetDrawColor(const Color4d & c);
        Color4d GetDrawColor() const;
        void DrawRectangle(int x, int y, int w, int h, bool filled);
        void DrawLine(int x1, int y1, int x2, int y2);
        void DrawPixel(int x, int y);
        void Clear();
        void SetTitle(const std::string &title);
        std::string GetTitle() const;

        /**
         * Pre-run initialization.
         */
        void Startup();

        /**
         * Post-run de-initialization.
         */
        void Shutdown();

        /**
         * Runs this application.
         *
         * @param listener  receives render and other events
         */
        void Run(ISdlApplication1Listener &listener);


        double GetDeltaTime() const {
            return deltaTime;
        }

        double GetTime() const {
            return time;
        }

        void ResetTime(double t);

        double GetFps() const;

    private:
        /** The width used to initialize the window. */
        int width;

        /** The width used to initialize the window. */
        int height;

        /** The window title. */
        std::string title;

        /** The SDL window. */
        SDL_Window* window;

        /** The SDL renderer used to render primitives. */
        SDL_Renderer *renderer;

        /** Control variable used to determine termination of application. */
        bool running;

        /** The background color used by the renderer. */
        Color4d backgroundColor;

        /** The draw color used by the renderer. */
        Color4d drawColor;

        /** Used to translate the performance counter to seconds. */
        double performToSeconds;

        /** Used to measure the elapsed time. */
        uint64_t performCnt;

        /** The elapsed time since the last update. */
        double deltaTime;

        /** The absolute time. */
        double time;

        /** The average frames per second. */
        double fpsSum;

        /** Used to determine when the FPS should be updated. */
        double fpsUpdate;

        /** The average frames per second. */
        double fps;

        /** The number of frames since the last FPS update. */
        unsigned int cntFrames;

        /** Used to transfer keyboard events to keyboard mono-state. */
        Keyboard keyboard;

        /** Used to transfer mouse events to mouse mono-state. */
        Mouse mouse;

        /**
         * Initializes SDL and prerequisites for the application.
         */
        void Initialize();

        /**
         * Shuts SDL down and releases acquired resources.
         */
        void CleanUp();

        /**
         * Renders the content to screen.
         *
         * @param listener  receives the OnRender event
         */
        void Render(ISdlApplication1Listener &listener);

        /**
         * Processes the event queue.
         *
         * @param listener  receives selected events
         */
        void ProcessEvents(ISdlApplication1Listener& listener);

        /**
         * Updates elapsed time.
         */
        void UpdateTime();

        /**
         * Updates the FPS counter.
         */
        void UpdateFps();

    };

}