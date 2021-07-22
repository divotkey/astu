/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Service.h"

namespace astu {

    /**
     * Provides core functionality for SDL.
     * 
     * @ingroup sdl_group
     */
    class SdlService final : public Service {
    public:

        /**
         * Constructor.
         * 
         * @param debug     fosters debugging, enables debug log messages, etc.
         * @param verbose   enables verbose log messages, only effective, when debug is true
         */
        SdlService(bool debug = false, bool verbose = false);

    protected:

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:
        /** Enables debug logging messages etc. */
        bool debugMode;

        /** Defines whether log messages should be verbose. */
        bool verboseLogging;
    };

} 