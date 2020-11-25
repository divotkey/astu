/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <SDL2/SDL.h>
#include "AstUtils0.h"
#include "SdlApplication.h"

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
    double deltaTime;

    /** The absolute time. */
    double time;

    /** The background color of the application window. */
    uint8_t bgColor[3] = {0, 0, 0};

    /** The background color of the application window. */
    uint8_t drawColor[4] = {255, 255, 255, 255};
}

int InitApp(int width, int height, const char title[])
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
        return GetLastError();
    }

    astu::renderer = SDL_CreateRenderer(
        astu::window, 
        -1, 
        SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC
        );

    if (!astu::renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't create SDL Renderer: %s", SDL_GetError());        
        QuitApp();
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

bool IsAppTerminated()
{
    return astu::terminated;
}

void ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            astu::terminated = true;
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
        static_cast<uint8_t>(SDL_ALPHA_OPAQUE)
        );
}

void RenderApp()
{
    SDL_RenderPresent(astu::renderer);   
}

void UpdateTime() 
{
    uint64_t now = SDL_GetPerformanceCounter();
    astu::deltaTime = (now - astu::performCnt) * astu::performToSeconds;
    astu::performCnt = now;
    astu::time += astu::deltaTime;
}

void UpdateApp()
{
    ProcessEvents();
    RenderApp();
    UpdateTime();
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
    astu::drawColor[3] = static_cast<uint8_t>(b);

    SDL_SetRenderDrawColor(
        astu::renderer, 
        static_cast<uint8_t>(astu::drawColor[0]), 
        static_cast<uint8_t>(astu::drawColor[1]), 
        static_cast<uint8_t>(astu::drawColor[2]),
        static_cast<uint8_t>(astu::drawColor[3])
        );

    return NO_ERROR;
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
}

int RenderPoint(double x, double y)    
{
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    SDL_RenderDrawPoint(astu::renderer, x, y);
    return NO_ERROR;
}

int RenderLine(double x1, double y1, double x2, double y2)
{
    if (!astu::renderer) {
        SetLastError(SDL_ERROR);
        SetErrorDetails("Application not initialized");
        return GetLastError();
    }

    if (SDL_RenderDrawLine(astu::renderer, x1, y1, x2, y2)) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't render line: %s", SDL_GetError());        
        SetLastError(SDL_ERROR);
        SetErrorDetails(SDL_GetError());
        return GetLastError();
    }

    return NO_ERROR;
}

double GetDeltaTime()
{
    return astu::deltaTime;
}
