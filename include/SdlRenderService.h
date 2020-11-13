/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "UpdateService.h"

// Forward declaration.
struct SDL_Renderer;

namespace astu {

    /**
     * Initializes and maintains an SDL renderer.
     * 
     * This service has a depencendy to SdlVideoService because
     * creating an SDL renderer requires access to the SDL window.
     */
    class SdlRenderService : public UpdatableBaseService {
    public:

        /**
         * Constructor.
         */
        SdlRenderService();

        /**
         * Virtual destructor.
         */
        virtual ~SdlRenderService() {}

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
        virtual void OnUpdate() override;

    private:
        /** The SDL renderer. */
        SDL_Renderer* renderer;

        /**
         * Logs some additional information about the used renderer.
         */
        void LogRendererInfo();
    };

} 