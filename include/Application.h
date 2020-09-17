/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include "Color.h"

namespace astu {

    // Forward declaration.
    class ApplicationImpl;

    class Application {
    public:

        /**
         * Constructor.
         */
        Application();

        /**
         * Virtual destructor.
         */
        virtual ~Application();

        /**
         * Sets the width of the application window.
         * 
         * @param w the width in pixels
         */
        void SetWidth(int w);

        /**
         * Sets the height of the application window.
         * 
         * @param h the height in pixels
         */
        void SetHeight(int h);

        /**
         * Returns the width of the application window.
         * 
         * @return the width in pixels
         */
        int GetWidth() const;

        /**
         * Returns the width of the application window.
         * 
         * @return the height in pixels
         */
        int GetHeight() const;

        /**
         * Sets the background color of the application window.
         * 
         * @param c the background color
         */
        void SetBackgroundColor(const Color & c);

        /**
         * Returns the background color for the application window.
         * 
         * @param the current background color
         */
        Color GetBackgroundColor() const;

        /**
         * Sets the draw color of the application window.
         * 
         * @param c the draw color
         */
        void SetDrawColor(const Color & c);

        /**
         * Returns the draw color for the application window.
         * 
         * @param the current draw color
         */
        Color GetDrawColor() const;

        /**
         * Draws a rectangle with the current draw color.
         * 
         * @param x         the x location of the upper left corner
         * @param y         the y location of the upper left corner
         * @param w         the width of the rectangle
         * @param h         the height of the rectangle
         * @param filled    `true` to render the rectangle filled
         */
        void DrawRectangle(int x, int y, int w, int h, bool filled = true);

        /**
         * Draws a line with the current draw color.
         * 
         * @parm x1 the x-coordinate of the start point
         * @parm y1 the y-coordinate of the start point
         * @parm x2 the x-coordinate of the end point
         * @parm y2 the y-coordinate of the end point
         */
        void DrawLine(int x1, int y1, int x2, int y2);

        /**
         * Clears the canvas using the current clear color.
         */
        void Clear();

        /**
         * Moves the content of the graphical content vertical.
         * If entire graphical content will be moved the specified number of
         * pixels. If the number of pixels is positive, the content will be
         * moved upwards. If the number of pixels is negative, the content 
         * will be moved downwards.
         * 
         * @param delta the number of pixels to move the content
         * @param clear determines whether the new empty spaced should be
         *      cleared using the current background color 
         */
        void MoveVertical(int delta, bool clear = false);

        /**
         * Starts this application.
         */ 
        void Run();

        /**
         * Returns the elapsed time since the last update.
         * 
         * @return the elapsed time in seconds
         */
        double GetDeltaTime() const;

        /**
         * Returns the absolute time.
         * 
         * @return the absolute time in seconds.
         */
        double GetTime() const;

        /**
         * Resets the absolute time to the specified value.
         * 
         * @param t the absolute time in seconds
         */
        void ResetTime(double t = 0);

    protected:

        /** 
         * Called by this base class to render application specific content.
         */ 
        virtual void OnRender() {}

        /**
         * Called by this base class when the application is started.
         */
        virtual void OnStartup() {}

        /**
         * Called by this base class when the application is shutdown.
         */
        virtual void OnShutdown() {}

    private:
        /** Pointer to the implementation, see PImpl C++ Idiom. */
        std::unique_ptr<ApplicationImpl> impl;
    };

}