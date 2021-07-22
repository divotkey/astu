/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "Color.h"

namespace astu {

    /**
     * Interface for main render facility.
     * 
     * @ingroup srv_group
     */
    class RenderService {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~RenderService() {}

        /**
         * Returns the background color.
         * 
         * @return the current background color
         */
        virtual const Color4f& GetBackgroundColor() const = 0;
    
        /**
         * Sets the background color of the main application window.
         * 
         * @param bg    the background color
         */
        virtual void SetBackgroundColor(const Color4f& bg) = 0;

    };
    
} // end of namespace