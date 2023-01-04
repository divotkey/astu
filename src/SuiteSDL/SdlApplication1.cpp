/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "SuiteSDL/SdlApplication1.h"
#include "SuiteSDL/ApplicationImpl.h"

namespace astu {

    SdlApplication1::SdlApplication1()
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
    SdlApplication1::~SdlApplication1()
    {
        // Intentionally left empty.
    }

    void SdlApplication1::SetWidth(int w)
    {
        impl->SetWidth(w);
    }

    void SdlApplication1::SetHeight(int w)
    {
        impl->SetHeight(w);
    }

    int SdlApplication1::GetWidth() const
    {
        return impl->GetWidth();
    }

    int SdlApplication1::GetHeight() const
    {
        return impl->GetHeight();
    }

    void SdlApplication1::SetBackgroundColor(const Color4d & c)
    {
        impl->SetBackgroundColor(c);
    }

    Color4d SdlApplication1::GetBackgroundColor() const
    {
        return impl->GetBackgroundColor();
    }

    void SdlApplication1::SetDrawColor(const Color4d & c)
    {
        impl->SetDrawColor(c);
    }

    Color4d SdlApplication1::GetDrawColor() const
    {
        return impl->GetDrawColor();
    }

    void SdlApplication1::DrawRectangle(int x, int y, int w, int h, bool filled)
    {
        impl->DrawRectangle(x, y, w, h, filled);
    }

    void SdlApplication1::DrawLine(int x1, int y1, int x2, int y2)
    {
        impl->DrawLine(x1, y1, x2, y2);
    }

    void SdlApplication1::DrawPixel(int x, int y)
    {
        impl->DrawPixel(x, y); 
    }

    void SdlApplication1::Clear()
    {
        impl->Clear();
    }

    double SdlApplication1::GetFps() const
    {
        return impl->GetFps();
    }

    void SdlApplication1::SetTitle(const std::string & title)
    {
        impl->SetTitle(title);
    }

    std::string SdlApplication1::GetTitle() const
    {
        return impl->GetTitle();
    }

    void SdlApplication1::Run()
    {
        impl->Startup();
        OnStartup();
        impl->Run(*this);
        OnShutdown();
        impl->Shutdown();
    }

    double SdlApplication1::GetDeltaTime() const
    {
        return impl->GetDeltaTime();
    }

    double SdlApplication1::GetTime() const
    {
        return impl->GetTime();
    }

    void SdlApplication1::ResetTime(double t)
    {
        impl->ResetTime(t);
    }

}