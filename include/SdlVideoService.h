/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>
#include "Service.h"
#include "IWindowManager.h"

// Forward declaration.
struct SDL_Window;

namespace astu {

    class SdlVideoService : public BaseService, public IWindowManager {
    public:

        /**
         * Constructor.
         */
        SdlVideoService();

        /**
         * Virtual destructor.
         */
        virtual ~SdlVideoService() {}

        // Inherited via IWindowManager
        virtual void SetSize(int width, int height) override;
        virtual int GetWidth() const override;
        virtual int GetHeight() const override;
        virtual void SetTitle(const std::string & title) override;
        virtual const std::string & GetTitle() const override;


    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** The SDL window. */
        SDL_Window* window;

        /** The window width. */
        int winWidth;

        /** The window height. */
        int winHeight;

        /** The window title. */
        std::string winTitle;


        void CleanUp();
    };

} 