/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <iostream>
#include "VersionInfo.h"
#include "AstUtilsConfig.h"
#include "ApplicationImpl.h"

#define FPS_UPDATE_INTERVAL 1.0

namespace astu {

    const Color ApplicationImpl::kDefaultBackgroundColor = Color::CreateFromRgb(0x77, 0x88, 0x99, 0xff);

    const Color ApplicationImpl::kDefaultDrawColor = Color::CreateFromRgb(0xD3, 0xD3, 0xD3, 0xff);

    ApplicationImpl::ApplicationImpl()
        : width(kDefaultWidth)
        , height(kDefaultHeight)
        , window(nullptr)
        , renderer(nullptr)
        , title(GetVersionInfo())
        , backgroundColor(kDefaultBackgroundColor)
        , drawColor(kDefaultDrawColor)
        , deltaTime(0)
        , time(0)
        , fps(0)
    {
        // Intentionally left empty.
    }

    void ApplicationImpl::SetWidth(int w)
    {
        if (w <= 0) {
            throw std::domain_error("Width of application window must be greater zero, got " 
                + std::to_string(w));
        }

        width = w;
    }

    void ApplicationImpl::SetHeight(int h)
    {
        if (h <= 0) {
            throw std::domain_error("Height of application window must be greater zero, got " 
                + std::to_string(h));
        }

        height = h;
    }

    int ApplicationImpl::GetWidth() const
    {
        return width;
    }

    int ApplicationImpl::GetHeight() const
    {
        return height;
    }

    void ApplicationImpl::SetBackgroundColor(const Color & c)
    {
        backgroundColor = c;
    }

    Color ApplicationImpl::GetBackgroundColor() const
    {
        return backgroundColor;
    }

    void ApplicationImpl::SetDrawColor(const Color & c)
    {
        drawColor = c;
        if (renderer) {
            SDL_SetRenderDrawColor(
                renderer, 
                static_cast<uint8_t>(drawColor.r * 255), 
                static_cast<uint8_t>(drawColor.g * 255), 
                static_cast<uint8_t>(drawColor.b * 255), 
                static_cast<uint8_t>(drawColor.a * 255));
        }
    }

    Color ApplicationImpl::GetDrawColor() const
    {
        return drawColor;
    }    

    double ApplicationImpl::GetFps() const
    {
        return fps;
    }

    void ApplicationImpl::DrawRectangle(int x, int y, int w, int h, bool filled)
    {
        if (!renderer) {
            throw std::logic_error("Application not running");
        }

        SDL_Rect rect;
        rect.x = x; rect.y = y;
        rect.w = w;  rect.h = h;

        if (filled) {
            if (SDL_RenderFillRect(renderer, &rect)) {
                SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't render rectangle: %s", SDL_GetError());        
            }
        } else {
            if (SDL_RenderDrawRect(renderer, &rect)) {
                SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't render rectangle: %s", SDL_GetError());        
            }
        }
    }

    void ApplicationImpl::DrawLine(int x1, int y1, int x2, int y2)
    {
        if (!renderer) {
            throw std::logic_error("Application not running");
        }

        if (SDL_RenderDrawLine(renderer, x1, y1, x2, y2)) {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't render line: %s", SDL_GetError());        
        }
    }

    void ApplicationImpl::DrawPixel(int x, int y)    
    {
        if (!renderer) {
            throw std::logic_error("Application not running");
        }

        SDL_RenderDrawPoint(renderer, x, y);
    }

    void ApplicationImpl::Clear()
    {
        if (!renderer) {
            throw std::logic_error("Application not running");
        }
        
        SDL_SetRenderDrawColor(
            renderer, 
            static_cast<uint8_t>(backgroundColor.r * 255), 
            static_cast<uint8_t>(backgroundColor.g * 255), 
            static_cast<uint8_t>(backgroundColor.b * 255), 
            SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(
            renderer, 
            static_cast<uint8_t>(drawColor.r * 255), 
            static_cast<uint8_t>(drawColor.g * 255), 
            static_cast<uint8_t>(drawColor.b * 255), 
            SDL_ALPHA_OPAQUE);
    }

    void ApplicationImpl::SetTitle(const std::string &title)
    {
        this->title = title;
        if (window) {
            SDL_SetWindowTitle(window, title.c_str());
        }
    }

    std::string ApplicationImpl::GetTitle() const
    {
        return title;
    }

    void ApplicationImpl::Run(std::function<void()> renderCallback)
    {
        Initialize();
        running = true;
        while (running) {
            UpdateTime();
            UpdateFps();            
            ProcessEvents();
            Render(renderCallback);
        }
        CleanUp();
    }

    void ApplicationImpl::ResetTime(double t)
    {
        time = t;
    }

    void ApplicationImpl::Initialize()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());        
            throw std::runtime_error(SDL_GetError());
        }    

        window = SDL_CreateWindow(
            title.c_str(), 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN
        );

        if (!window) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't create SDL Window: %s", SDL_GetError());        
            CleanUp();
            throw std::runtime_error(SDL_GetError());
        }

        renderer = SDL_CreateRenderer(
            window, 
            -1, 
            SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC
            );

        if (!renderer) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't create SDL Renderer: %s", SDL_GetError());        
            CleanUp();
            throw std::runtime_error(SDL_GetError());
        }            

        if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't set blend mode for SDL Renderer: %s", SDL_GetError());        
            CleanUp();
            throw std::runtime_error(SDL_GetError());
        }            

        performToSeconds = 1.0 / SDL_GetPerformanceFrequency();
        performCnt = SDL_GetPerformanceCounter();

        fpsSum = 0;
        fpsUpdate = FPS_UPDATE_INTERVAL;
        cntFrames = 0;
    }

    void ApplicationImpl::CleanUp()
    {
        if(renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
    }

    void ApplicationImpl::Render(std::function<void()> renderCallback)
    {
        renderCallback();
        SDL_RenderPresent(renderer);
    }

    void ApplicationImpl::ProcessEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_DROPTEXT:
                std::cout << "drop text" << std::endl;
                break;

            case SDL_DROPBEGIN:
                std::cout << "drop begin" << std::endl;
                break;

            case SDL_DROPCOMPLETE:
                std::cout << "drop complete" << std::endl;
                break;

            case SDL_DROPFILE:
                std::cout << "file dropped: '" << event.drop.file << "'" << std::endl;
                SDL_free(event.drop.file);
                break;
            }
        } 
    }    

    void ApplicationImpl::UpdateTime() 
    {
        uint64_t now = SDL_GetPerformanceCounter();
        deltaTime = (now - performCnt) * performToSeconds;
        performCnt = now;
        time += deltaTime;
    }

    void ApplicationImpl::UpdateFps()
    {
        ++cntFrames;
        fpsSum += deltaTime;
        fpsUpdate -= deltaTime;
        if (fpsUpdate <= 0) {
            fps = cntFrames / fpsSum;
            cntFrames = 0;
            fpsSum = 0;
            fpsUpdate = FPS_UPDATE_INTERVAL;
        }
    }

}