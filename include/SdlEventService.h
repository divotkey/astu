/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "Service.h"

namespace astu {

    class SdlEventService : public BaseService {
    public:

        /**
         * Constructor.
         */
        SdlEventService();

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:


        // Inherited via IUpdatable
    };

} 