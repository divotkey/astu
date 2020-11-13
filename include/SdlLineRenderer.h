/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>
#include "UpdateService.h"
#include "ILineRenderer.h"

// Forward declaration.
// struct SDL_Window;

namespace astu {

    class SdlLineRenderer : public UpdatableBaseService, public ILineRenderer {
    public:

        /**
         * Constructor.
         */
        SdlLineRenderer();

        /**
         * Virtual destructor.
         */
        virtual ~SdlLineRenderer() {}

        // Inherited via ILineRenderer
        virtual void DrawLine(const Vector2 & p1, const Vector2 & p2) override;

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
    
    };

} 