/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include "UpdateService.h"

namespace astu {

    class SdlEventService : public UpdatableBaseService {
    public:

        /**
         * Constructor.
         * 
         * @param priority    the priority used to update this service
         */
        SdlEventService(int priority = 0);

        /**
         * Virtual destructor.
         */
        ~SdlEventService() {}

        /**
         * Return whether a quit-signal has been detected.
         * 
         * @return `true` if a quit-signal has been detected
         */
        bool IsQuit() const;

        /**
         * Clears the quit-signal if set.
         */
        void ClearQuit();

    protected:

        // Inherited via UpdatableBaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
        virtual void OnUpdate() override;

    private:
        /** Determines whether a quit-signal has beed detected. */
        bool quit;
    };

} 