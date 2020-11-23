/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "Service.h"

namespace astu {

    /**
     * Provides core functionality for SDL.
     * 
     * @ingroup sdl_group
     */
    class SdlService : public BaseService {
    public:

        /**
         * Constructor.
         */
        SdlService(bool debug = false);

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** Enabmes debug logging messages etc. */
        bool debugMode;
    };

} 