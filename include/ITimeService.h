/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * Interface for an services which keepts track of time.
     * 
     * @ingroup srv_group
     */
    class ITimeService {
    public:

        /**
         * Virtual destructor.
         */
        ~ITimeService() {}

        /**
         * Returns the elapsed time since the last update.
         * 
         * @param double    the elapsed time in seconds
         */
        virtual double GetElapsedTime() const = 0;
    };

} 