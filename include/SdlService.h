/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
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
         * 
         * @param debug     fosters debugging, enables debug log messages, etc.
         * @param verbose   enables verbose log messages, only effective, when debug is true
         */
        SdlService(bool debug = false, bool verbose = false);

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** Enabmes debug logging messages etc. */
        bool debugMode;

        /** Defines whether log messages should be verbose. */
        bool verboseLogging;
    };

} 