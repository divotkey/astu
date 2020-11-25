/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

/**
 * @file
 * @brief This file defines public functions related to SDL-based applications using AST utilities API-Level 0.
 */

#pragma once

/**
 * Initializes the SDL-based application.
 * 
 * This function opens the main appplication window and carries out
 * some internal initializaiton procedures and acquire required resources.
 * 
 * @param width the width of the main window in pixels
 * @param height the width of the main window in pixels
 * @param title the title of the application window
 * @return an error code in case the application could not be initialized, zero
 *  otherwise
 * 
 * **Example Usage**
 * 
 * ```
 * if (InitApp(640, 480) != NO_ERROR) {
 *   // TODO report error
 *   return -1;
 * }
 * 
 * ```
 * 
 * @ingroup sdl_group
 */
int InitApp(int width, int height, const char title[] = "ASTU/SDL Application");

/**
 * Terminates the application.
 * 
 * This functions closes the main window and releases all aquired
 * resources. In case the application hast not been initialized,
 * this function has no effect
 * 
 * @ingroup sdl_group
 */
void QuitApp();

/**
 * Updates the application.
 * 
 * This function must be called repeatedly in a loop until the application
 * is terminated by the user. Use IsAppTerminated to query whether
 * the user has requested to close the application window.
 * 
 * **Example Usage**
 * 
 * ```
 * while ( !IsAppTerminated() ) {
 *   UpdateApp();
 * }
 * ```
 * 
 * @ingroup sdl_group
 */
void UpdateApp();

/**
 * Returns `true` in case the user want's to terminate the application.
 * 
 * On most plattform the request to terminate the application happens
 * e.g., by pressing the close button of the application window.
 * 
 * @return `true` if the application should be terminated, `false` otherwise
 * 
 * @ingroup sdl_group
 */
bool IsAppTerminated();

/**
 * Clears the canvas of the application window.
 * 
 * @return an error code in case the canvas could not be cleared, zero otherwise
 * 
 * @ingroup sdl_group
 */
int ClearCanvas();

/**
 * Renders a line between two points sing the current render color. 
 * 
 * @param x1    the x-coordinate of the first point
 * @param y1    the y-coordinate of the first point
 * @param x2    the x-coordinate of the second point
 * @param y2    the y-coordinate of the second point
 * @return an error code in case the line could not be rendered, zero otherwise
 * 
 * @ingroup sdl_group
 */
int RenderLine(double x1, double y1, double x2, double y2);

/**
 * Renders a point using the current render color. 
 * 
 * @param x the x-coordinate of the point
 * @param y the y-coordinate of the point
 * @return an error code in case the line could not be rendered, zero otherwise
 * 
 * @ingroup sdl_group
 */
int RenderPoint(double x, double y);

/**
 * Sets the render color for subsequent render calls.
 * 
 * @param r the red component of the render color [0, 255]
 * @param g the green component of the render color [0, 255]
 * @param b the blue component of the render color [0, 255]
 * @param a the alpha component of the render color [0, 255]
 * @return an error code in case the color could not be set, zero otherwise
 * 
 * **Example Usage**
 * 
 * ```
 * // Set render color to orange, full opaque.
 * SetRenderColor(255, 165, 0, 255);
 * 
 * // Set render color to yellow, 50% transparent.
 * SetRenderColor(255, 255, 0, 128);
 * 
 * // Set render color to white, full opaque.
 * SetRenderColor(255, 255, 255);
 * ```
 * 
 * @ingroup sdl_group
 */
int SetRenderColor(int r, int g, int b, int a = 255);

/**
 * Sets the background color of the application window.
 * 
 * The background color does not have an alpha channel and cannot
 * be transparent.
 * 
 * @param r the red component of the render color [0, 255]
 * @param g the green component of the render color [0, 255]
 * @param b the blue component of the render color [0, 255]
 * @return an error code in case the color could not be set, zero otherwise
 * 
 * @ingroup sdl_group
 */
int SetBackgroundColor(int r, int g, int b);

/**
 * Returns the epalsed time since the last update.
 * 
 * This method returns the elapsed time since the last call to
 * UpdateApp. 
 * 
 * @return the delta time in seconds
 * 
 * @ingroup sdl_group
 */
double GetDeltaTime();