/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/InteractiveApplication.h"

namespace astu {

    /**
     * Simplified the requried startup code for SDL-based interactive applications.
     * 
     * @ingroup sdl_group
     */
    class SdlApplication : public InteractiveApplication {
    public:

        /**
         * Constructor.
         */
        SdlApplication();

    private:

        /**
         * Adds required SDL specific services.
         */
        void AddSdlServices();
    };

} // end of namespace