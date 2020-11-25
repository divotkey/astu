/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include <string>
#include "Color.h"

namespace astu {

    // Forward declaration.
    class ApplicationImpl;

    /**
     * 
     */
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
         * @param x1 the x-coordinate of the start point
         * @param y1 the y-coordinate of the start point
         * @param x2 the x-coordinate of the end point
         * @param y2 the y-coordinate of the end point
         */
        void DrawLine(int x1, int y1, int x2, int y2);

        /**
         * Draws a pixel with the current draw color.
         * 
         * @param x the x-coordinate of the start point
         * @param y the y-coordinate of the start point
         */
        void DrawPixel(int x, int y);

        /**
         * Clears the canvas using the current clear color.
         */
        void Clear();

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

        /**
         * Returns the average frames per seconds (FPS).
         * 
         * @return the FPS in seconds
         */
        double GetFps() const;

        /**
         * Sets the title of the application window.
         * 
         * @param title the title of the window
         */
        void SetTitle(const std::string & title);

        /**
         * Returns the title of the application window.
         * 
         * @return the title of the application window
         */
        std::string GetTitle() const;

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