/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/InteractiveApplication.h"

namespace astu {

    /**
     * Simplified the required startup code for SDL-based interactive applications.
     * 
     * @ingroup sdl_group
     */
    class SdlApplication : public InteractiveApplication {
    public:

        /**
         * Constructor.
         */
        SdlApplication();

        /**
         * Specifies whether to add services required for 2D renderings.
         * 
         * @param b set to `true` to add 2D services
         */
        void AddServices2D(bool b);

        /**
         * Returns whether services required for 2D renderings will be added.
         *  
         * @return `true` if 2D services will be added
         */
        bool IsServices2D() const;

    protected:

        // Inherited via InteractiveApplication
        virtual void ConfigureApplication() override;
        virtual void Cleanup() override;


    private:
        /** Whether to add services required for 2D renderings. */
        bool add2DServices;

        /**
         * Adds required SDL specific services.
         */
        void AddSdlServices();

        /**
         * Adds required SDL services specific for 2D rendering.
         */
        void AddSdl2DServices();

        /**
         * Removes required SDL services specific for 2D rendering.
         */
        void RemoveSdl2DServices();
    };

} // end of namespace