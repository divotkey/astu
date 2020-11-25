/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "Service.h"

namespace astu {

    class SdlAudioService : public BaseService {
    public:

        /**
         * Constructor.
         */
        SdlAudioService();

        /**
         * Virtual destructor.
         */
        virtual ~SdlAudioService() {}

    protected:

        // Inherited via Base Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

    private:


        /**
         * Logs some additional information about audio driver and device.
         */
        void LogAudioInfo();


        /**
         * Releases recources. 
         */
        void CleanUp();
    };

}