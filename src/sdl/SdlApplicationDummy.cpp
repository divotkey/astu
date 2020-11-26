/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "AstUtils0.h"
#include "SdlApplication.h"

int SetNoSDLSupportError()
{
    SetLastError(SDL_ERROR);
    SetErrorDetails("SDL support not activated. Run cmake command in build folder: cmake -DWITH_SDL=true ../");
    return GetLastError();
}

int InitApp(int width, int height, const char title[], bool vsync)
{
    return SetNoSDLSupportError();
}

void QuitApp()
{
    // Intentionally left empty.
}

int SetWindowTitle(const char title[])
{
    return SetNoSDLSupportError();
}

bool IsAppTerminated()
{
    return true;
}

int ClearCanvas()
{
    return SetNoSDLSupportError();
}

void UpdateApp()
{
}

int SetRenderColor(int r, int g, int b, int a) 
{
    return SetNoSDLSupportError();
}

int SetBackgroundColor(int r, int g, int b)
{
    return SetNoSDLSupportError();
}

int RenderPoint(double x, double y)    
{
    return SetNoSDLSupportError();
}

int RenderLine(double x1, double y1, double x2, double y2)
{
    return SetNoSDLSupportError();
}

double GetDeltaTime()
{
    return 0;
}

double GetFps()
{
    return 0;
}

int GetCursorX()
{
    return 0;
}

int GetCursorY()
{
    return 0;
}

bool IsMouseButtonPressed(int button)
{
    return false; 
}
