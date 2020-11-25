/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "ApplicationImpl.h"
#include "Application.h"

namespace astu {

    Application::Application()
        : impl(std::make_unique<ApplicationImpl>())
    {
        // Intentionally left empty.
    }

    // Note: the (virtual) destructor must be defined int the cpp file
    // in order to let the unique pointer know how to destroy the 
    // implementation file. In other words, a unique pointer
    // requires the declaration of what it has to destroy and we use
    // a forward declaration for the implementation file in the header,
    // to fully encapsulate the details of the implementation.
    Application::~Application()
    {
        // Intentionally left empty.
    }

    void Application::SetWidth(int w)
    {
        impl->SetWidth(w);
    }

    void Application::SetHeight(int w)
    {
        impl->SetHeight(w);
    }

    int Application::GetWidth() const
    {
        return impl->GetWidth();
    }

    int Application::GetHeight() const
    {
        return impl->GetHeight();
    }

    void Application::SetBackgroundColor(const Color & c)
    {
        impl->SetBackgroundColor(c);
    }

    Color Application::GetBackgroundColor() const
    {
        return impl->GetBackgroundColor();
    }

    void Application::SetDrawColor(const Color & c)
    {
        impl->SetDrawColor(c);
    }

    Color Application::GetDrawColor() const
    {
        return impl->GetDrawColor();
    }

    void Application::DrawRectangle(int x, int y, int w, int h, bool filled)
    {
        impl->DrawRectangle(x, y, w, h, filled);
    }

    void Application::DrawLine(int x1, int y1, int x2, int y2)
    {
        impl->DrawLine(x1, y1, x2, y2);
    }

    void Application::DrawPixel(int x, int y)
    {
        impl->DrawPixel(x, y); 
    }

    void Application::Clear()
    {
        impl->Clear();
    }

    double Application::GetFps() const
    {
        return impl->GetFps();
    }

    void Application::SetTitle(const std::string & title)
    {
        impl->SetTitle(title);
    }

    std::string Application::GetTitle() const
    {
        return impl->GetTitle();
    }

    void Application::Run()
    {
        OnStartup();
        impl->Run([this](){ OnRender(); });
        OnShutdown();
    }

    double Application::GetDeltaTime() const
    {
        return impl->GetDeltaTime();
    }

    double Application::GetTime() const
    {
        return impl->GetTime();
    }

    void Application::ResetTime(double t)
    {
        impl->ResetTime(t);
    }

}