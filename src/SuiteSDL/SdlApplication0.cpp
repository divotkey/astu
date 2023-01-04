/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Library-Local includes
#include "Math/Vector2.h"
#include "Math/Matrix3.h"
#include "AstUtils0.h"
#include "SuiteSDL/SdlApplication0.h"

// SDL includes
#include <SDL2/SDL.h>

// C++ Standard Library includes
#define _USE_MATH_DEFINES
#include <array>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>

#define FPS_UPDATE_INTERVAL 1.0
#define NUM_MOUSE_BUTTONS   5
#define MAX_DELTA_TIME      (1.0 / 15.0)

/////////////////////////////////////////////////
/////// Globals.
/////////////////////////////////////////////////

namespace astu {

    /** The SDL window. */
    SDL_Window* window = nullptr;

    /** The SDL renderer used to render primitives. */
    SDL_Renderer *renderer = nullptr;

    /** Indicates whether the user wants to close the application window. */
    bool terminated;

    /** Used to translate the performance counter to seconds. */
    double performToSeconds;

    /** Used to measure the elapsed time. */
    uint64_t performCnt;

    /** The elapsed time since the last update. */
    double deltaTime = 1.0 / 60.0;

    /** The absolute time. */
    double time;

    /** The background color of the application window. */
    uint8_t bgColor[3] = {0, 0, 0};

    /** The background color of the application window. */
    uint8_t drawColor[4] = {255, 255, 255, 255};

    /** The global transformation used for rendering. */
    Matrix3<double> transform;

    /** The current x-coordinate of the mouse cursor. */
    int cursorX = 0;

    /** The current x-coordinate of the mouse cursor. */
    int cursorY = 0;

    /** The pressed states of the mouse buttons. */
    std::array<bool, NUM_MOUSE_BUTTONS> buttons = {false, false, false, false, false};

    /** The clicked states of the mouse buttons. */
    std::array<bool, NUM_MOUSE_BUTTONS> buttonsClicked =  {false, false, false, false, false};

    struct FpsStats {

        FpsStats() : fpsSum(0), fpsUpdate(FPS_UPDATE_INTERVAL), fps(60), cntFrames(0) {}

        /** The average frames per second. */
        double fpsSum;

        /** Used to determine when the FPS should be updated. */
        double fpsUpdate;

        /** The average frames per second. */
        double fps;

        /** The number of frames since the last FPS update. */
        unsigned int cntFrames;
    } ;

    FpsStats fpsStats;

    /** Used to generate fps string. */
    std::ostringstream fpsStringStream;

    /** Stores the string output of the GetFpsString() method. */
    std::string fpsString;

    void RenderApp()
    {
        SDL_RenderPresent(astu::renderer);   
    }

    void UpdateTime() 
    {
        uint64_t now = SDL_GetPerformanceCounter();
        astu::deltaTime = (now - astu::performCnt) * astu::performToSeconds;
        astu::performCnt = now;
        astu::time += GetDeltaTime();
    }

    void UpdateFps()
    {
        ++astu::fpsStats.cntFrames;
        astu::fpsStats.fpsSum += astu::deltaTime;
        astu::fpsStats.fpsUpdate -= astu::deltaTime;
        if (astu::fpsStats.fpsUpdate <= 0) {
            astu::fpsStats.fps = astu::fpsStats.cntFrames / astu::fpsStats.fpsSum;
            astu::fpsStats.cntFrames = 0;
            astu::fpsStats.fpsSum = 0;
            astu::fpsStats.fpsUpdate = FPS_UPDATE_INTERVAL;
        }
    }    

    int TranslateButton(int sdlIdx) {
        switch (sdlIdx) {
        case SDL_BUTTON_LEFT:
            return 0;
        case SDL_BUTTON_MIDDLE:
            return 1;
        case SDL_BUTTON_RIGHT:
            return 2;
        case SDL_BUTTON_X1:
            return 3;
        case SDL_BUTTON_X2:
            return 4;

        default:
            return -1;
        }
    }

    void SetButtonState(int sdlIdx, bool pressed) {
        int idx = TranslateButton(sdlIdx);
        if (idx >= 0 && idx <= static_cast<int>(astu::buttons.size())) {
            buttons[idx] = pressed;
        }
    }

    void SetButtonClickedState(int sdlIdx, bool pressed) {
        int idx = TranslateButton(sdlIdx);
        if (idx >= 0 && idx <= static_cast<int>(astu::buttons.size())) {
            buttonsClicked[idx] = pressed;
        }
    }

    void ClearButtonClickedState()
    {
        for (auto & button : buttonsClicked) {
            button = false;
        }
    }

    void ProcessEvents()
    {
        ClearButtonClickedState();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                astu::terminated = true;
                break;

            case SDL_MOUSEMOTION:
                astu::cursorX = event.motion.x;
                astu::cursorY = event.motion.y;
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                SetButtonState(event.button.button, true);
                SetButtonClickedState(event.button.button, true);
                break;

            case SDL_MOUSEBUTTONUP:
                SetButtonState(event.button.button, false);
                break;

            case SDL_DROPTEXT:
                break;

            case SDL_DROPBEGIN:
                break;

            case SDL_DROPCOMPLETE:
                break;

            case SDL_DROPFILE:
                SDL_free(event.drop.file);
                break;
            }
        }    
    }


} // end of namespace

int InitApp(int width, int height, const char title[], bool vsync)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());        
        SetLastError(SDL_ERROR);
        SetErrorDetails(SDL_GetError());
        return GetLastError();
    }    

    astu::window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!astu::window) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't create SDL Window: %s", SDL_GetError());        
        SetLastError(SDL_ERROR);
        SetErrorDetails(SDL_GetError());
        QuitApp();
        return GetLastError();
    }

    int flags = SDL_RendererFlags::SDL_RENDERER_ACCELERATED;
    if (vsync) {
        flags |= SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC;
    }

    astu::renderer = SDL_CreateRenderer(
        astu::window, 
        -1,
        flags
        );

    if (!astu::renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't create SDL Renderer: %s", SDL_GetError());        
        SetLastError(SDL_ERROR);
        SetErrorDetails(SDL_GetError());
        QuitApp();
        return GetLastError();
    }

    if (SDL_SetRenderDrawBlendMode(astu::renderer, SDL_BLENDMODE_BLEND)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't set blend mode for SDL Renderer: %s", SDL_GetError());        
        SetLastError(SDL_ERROR);
        SetErrorDetails(SDL_GetError());
        QuitApp();
        return GetLastError();
    }            

    astu::terminated = false;
    astu::performToSeconds = 1.0 / SDL_GetPerformanceFrequency();
    astu::performCnt = SDL_GetPerformanceCounter();

    return NO_ERROR;
}

void QuitApp()
{
    if(astu::renderer) {
        SDL_DestroyRenderer(astu::renderer);
        astu::renderer = nullptr;
    }

    if (astu::window) {
        SDL_DestroyWindow(astu::window);
        astu::window = nullptr;
    }

    SDL_Quit();
    astu::terminated = true;
}

int GetWindowWidth()
{
    if (!astu::window) {
        return 0;
    }

    int width;
    int height;

    SDL_GetWindowSize(astu::window, &width, &height);
    return width;
}

int GetWindowHeight()
{
    if (!astu::window) {
        return 0;
    }

    int width;
    int height;

    SDL_GetWindowSize(astu::window, &width, &height);
    return height;
}

int SetWindowTitle(const char title[])
{
    if (!astu::window) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    SDL_SetWindowTitle(astu::window, title);

    return NO_ERROR;
}

bool IsAppTerminated()
{
    return astu::terminated;
}

int ClearCanvas()
{
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    SDL_SetRenderDrawColor(
        astu::renderer, 
        static_cast<uint8_t>(astu::bgColor[0]), 
        static_cast<uint8_t>(astu::bgColor[1]), 
        static_cast<uint8_t>(astu::bgColor[2]),
        static_cast<uint8_t>(SDL_ALPHA_OPAQUE)
        );

    SDL_RenderClear(astu::renderer);

    SDL_SetRenderDrawColor(
        astu::renderer, 
        static_cast<uint8_t>(astu::drawColor[0]), 
        static_cast<uint8_t>(astu::drawColor[1]), 
        static_cast<uint8_t>(astu::drawColor[2]),
        static_cast<uint8_t>(astu::drawColor[3])
        );

    return NO_ERROR;
}

void UpdateApp()
{
    astu::ProcessEvents();
    astu::RenderApp();
    astu::UpdateTime();
    astu::UpdateFps();
}

int SetRenderColor(int r, int g, int b, int a) 
{
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    astu::drawColor[0] = static_cast<uint8_t>(r);
    astu::drawColor[1] = static_cast<uint8_t>(g);
    astu::drawColor[2] = static_cast<uint8_t>(b);
    astu::drawColor[3] = static_cast<uint8_t>(a);

    SDL_SetRenderDrawColor(
        astu::renderer, 
        static_cast<uint8_t>(astu::drawColor[0]), 
        static_cast<uint8_t>(astu::drawColor[1]), 
        static_cast<uint8_t>(astu::drawColor[2]),
        static_cast<uint8_t>(astu::drawColor[3])
        );

    return NO_ERROR;
}

int SetRenderColor(int rgba)
{
    return SetRenderColor((rgba & 0xff000000) >> 24, (rgba & 0xff0000) >> 16, (rgba & 0xff00) >> 8, rgba & 0xff);   
}

int SetBackgroundColor(int r, int g, int b)
{
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    astu::bgColor[0] = static_cast<uint8_t>(r);
    astu::bgColor[1] = static_cast<uint8_t>(g);
    astu::bgColor[2] = static_cast<uint8_t>(b);

    return NO_ERROR;
}

int SetBackgroundColor(int rgb)
{
    return SetBackgroundColor((rgb & 0xff0000) >> 16, (rgb & 0xff00) >> 8, rgb & 0xff);
}

int RenderPoint(double x, double y)    
{
    // Test for proper initialization.
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    // Transform point according to global transformation matrix.
    auto p = astu::transform.TransformPoint(x, y);

    // Render transformed point.
    SDL_RenderDrawPoint(
            astu::renderer,
            static_cast<int>(p.x + 0.5),
            static_cast<int>(p.y + 0.5)
            );

    return NO_ERROR;
}

int RenderLine(double x1, double y1, double x2, double y2)
{
    // Test for proper initialization.
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    // Transform points according to global transformation matrix.
    auto p1 = astu::transform.TransformPoint(x1, y1);
    auto p2 = astu::transform.TransformPoint(x2, y2);

    // Render line.
    if (SDL_RenderDrawLine( 
        astu::renderer,
        static_cast<int>(p1.x + 0.5),
        static_cast<int>(p1.y + 0.5),
        static_cast<int>(p2.x + 0.5),
        static_cast<int>(p2.y + 0.5)))
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't render line: %s", SDL_GetError());        
        SetLastError(SDL_ERROR);
        SetErrorDetails(SDL_GetError());
        return GetLastError();
    }

    return NO_ERROR;
}

int RenderRectangle(double x, double y, double w, double h, bool filled)
{
    // Test for proper initialization.
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    // Transform center and dimensions.
    auto p0 = astu::transform.TransformPoint(x, y);
    auto v0 = astu::transform.TransformVector(w, 0);
    auto v1 = astu::transform.TransformVector(0, h);
    double wt = v0.Length();
    double ht = v1.Length();

    SDL_Rect rect;
    rect.x = static_cast<int>(p0.x - wt / 2);
    rect.y = static_cast<int>(p0.y - ht / 2);
    rect.w = static_cast<int>(wt);
    rect.h = static_cast<int>(ht);

    if (filled) {
        if (SDL_RenderFillRect(astu::renderer, &rect)) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't render rectangle: %s", SDL_GetError());        
            SetLastError(SDL_ERROR);
            SetErrorDetails(SDL_GetError());
            return GetLastError();
        }
    } else {
        if (SDL_RenderDrawRect(astu::renderer, &rect)) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't render rectangle: %s", SDL_GetError());        
            SetLastError(SDL_ERROR);
            SetErrorDetails(SDL_GetError());
            return GetLastError();
        }
    }

    return NO_ERROR;
}

int RenderRegularPolygon(double x, double y, double r, unsigned int n, double angle)
{
    // Verify parameters.

    if (r <= 0) {
        SetLastError(INVALID_PARAMETER);
        SetErrorDetails("Radius for n-gon must be greater zero");
        return GetLastError();
    }

    if (n <= 2) {
        SetLastError(INVALID_PARAMETER);
        SetErrorDetails("Number of vertices for n-gon must be greater 2");
        return GetLastError();
    }


    // Render polygon.

    double da = (2.0 * M_PI) / n;

    astu::Vector2<double> p0(r, 0);
    p0.Rotate(-angle);
    for (unsigned int i = 1; i <= n; ++i) {
        astu::Vector2<double> p1(p0);
        p1.Rotate(-da);

        auto res = RenderLine(x + p0.x, y + p0.y, x + p1.x, y + p1.y);
        if (res != NO_ERROR) {
            return res;
        }
        p0 = p1;
    }

    return NO_ERROR;
}

void ApplyRenderScaling(double sx, double sy)
{
    astu::transform.Scale(sx, sy);
}

void ApplyRenderTranslation(double tx, double ty)
{
    astu::transform.Translate(tx, ty);
}

void ApplyRenderRotation(double phi)
{
    astu::transform.Rotate(phi);
}

void ResetRenderTransform()
{
    astu::transform.SetToIdentity();
}

double GetDeltaTime()
{
    return (std::min)(MAX_DELTA_TIME, astu::deltaTime);
}

double GetAbsoluteTime()
{
    return astu::time;
}

void ResetAbsoluteTime()
{
    astu::time = 0;
}

double GetFps()
{
    return astu::fpsStats.fps;
}

const char* GetFpsString(const char* text, int precision)
{   
    astu::fpsStringStream.str("");
    if (text) {
        astu::fpsStringStream << text << " ";
    }

    astu::fpsStringStream.precision(precision);
    astu::fpsStringStream << std::fixed << GetFps();
    astu::fpsString = astu::fpsStringStream.str();
    return astu::fpsString.c_str();
}

int GetCursorX()
{
    return astu::cursorX;
}

int GetCursorY()
{
    return astu::cursorY;
}

bool IsMouseButtonPressed(int button)
{
    if (button >= 0 && button < static_cast<int>(astu::buttons.size())) {
        return astu::buttons[button];
    }

    return false;
}

bool IsMouseButtonClicked(int button)
{
    if (button >= 0 && button < static_cast<int>(astu::buttons.size())) {
        return astu::buttonsClicked[button];
    }

    return false;
}
