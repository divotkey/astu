/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <functional>
#include "Color.h"

namespace astu {

    class ApplicationImpl {
    public:
        /** The default window width. */
        static const int kDefaultWidth = 800;

        /** The default window height. */
        static const int kDefaultHeight = 600;

        /** The default background color. */
        static const Color kDefaultBackgroundColor;

        /** The default draw color. */
        static const Color kDefaultDrawColor;

        /**
         * Constructor.
         */
        ApplicationImpl();

        void SetWidth(int w);
        void SetHeight(int h);
        int GetWidth() const;
        int GetHeight() const;    
        void SetBackgroundColor(const Color & c);
        Color GetBackgroundColor() const;
        void SetDrawColor(const Color & c);
        Color GetDrawColor() const;
        void DrawRectangle(int x, int y, int w, int h, bool filled);
        void DrawLine(int x1, int y1, int x2, int y2);
        void Clear();
        void MoveVertical(int delta, bool clear);

        void Run(std::function<void()> renderCallback);


        double GetDeltaTime() const {
            return deltaTime;
        }

        double GetTime() const {
            return time;
        }

        void ResetTime(double t);

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
        Color backgroundColor;

        /** The draw color used by the renderer. */
        Color drawColor;

        /** Used to translate the performance counter to seconds. */
        double performToSeconds;

        /** Used to measure the elapsed time. */
        uint64_t performCnt;

        /** The elapsed time since the last update. */
        double deltaTime;

        /** The absolute time. */
        double time;


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
         */
        void Render(std::function<void()> renderCallback);

        /**
         * Processes the event queue.
         */
        void ProcessEvents();

        /**
         * Updates elapsed time.
         */
        void UpdateTime();

    };

}