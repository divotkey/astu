/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <cstdint>

// Local includes
#include "UpdateService.h"
#include "ITimeManager.h"

namespace astu {


    /**
     * Uses the SDL high performance timer to measure elapsed time.
     * 
     * @ingroup sdl_group 
     */
    class SdlTimeService final : public virtual Service, private Updatable, public ITimeManager {
    public:

        /**
         * Constructor.
         * 
         * @param priority    the priority used to update this service
         */
        SdlTimeService(int priority = 0);

        /**
         * Virtual destructor.
         */
        ~SdlTimeService() {}

        // Inherited via ITimeManager
        virtual double GetElapsedTime() const override;
        virtual double GetAbsoluteTime() const override;

    protected:

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via Updatable
        virtual void OnUpdate() override;

    private:        
        /** Used to translate the performance counter to seconds. */
        double performToSeconds;

        /** Used to measure the elapsed time. */
        uint64_t performCnt;

        /** The elapsed time since the last update. */
        double deltaTime;

        /** The absolute time. */
        double time;
    };

}