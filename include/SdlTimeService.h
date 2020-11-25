/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <cstdint>
#include "UpdateService.h"
#include "ITimeService.h"

namespace astu {


    /**
     * Uses the SDL high performance timer to measure elapsed time.
     * 
     * @ingroup sdl_group 
     */
    class SdlTimeService : public UpdatableBaseService, public ITimeService {
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

        // Inherited via ITimeService
        virtual double GetElapsedTime() const override;

    protected:

        // Inherited via UpdatableBaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
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