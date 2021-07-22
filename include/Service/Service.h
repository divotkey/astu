/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "ServiceManager.h"

// C++ Standard Library includes
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

namespace astu {

    /**
     * Base class for services.
     * 
     * It is recommended not to derive own services directly from this base
     * class, but to use BaseService instead. If this base class is used,
     * it must be derived as public virtual.     
     * */
    class Service {
    public:

        /** Describes the possible statuses that a service can be in. */
        enum Status {
            /** The service has not been started. */
            Stopped, 

            /** The service is currently paused. */
            Paused,

            /** The service has successfully been started. */
            Running
        };

        /** Default name for services. */
        static const std::string & DEFAULT_NAME;

        /**
         * Constructor.
         * 
         * Service names need not be unique and are mainly used for
         * information and debugging purposes.
         * 
         * @param name  the name of this service.
         */
        Service(const std::string & name = DEFAULT_NAME);

        /**
         * Pure virtual destructor, forces this class to be abstract.
         **/
        virtual ~Service() = 0;

        /**
         * Returns the name of this service.
         * 
         * @return this service's name
         */
        const std::string & GetName() const;

        /**
         * Returns the status of this service.
         * 
         * @return this service's status
         */
        Status GetStatus() const;

        /**
         * Adds a function which is called when this services is started.
         * 
         * @param func  the function to be called.
         */
        void AddStartupHook(std::function<void (void)> func);

        /**
         * Adds a function which is called when this service is shutdown.
         * 
         * @param func  the function to be called.
         */
        void AddShutdownHook(std::function<void (void)> func);

        /**
         * Adds a function which is called when this services is paused.
         * 
         * @param func  the function to be called.
         */
        void AddPauseHook(std::function<void (void)> func);

        /**
         * Adds a function which is called when this services is resumed.
         * 
         * @param func  the function to be called.
         */
        void AddResumeHook(std::function<void (void)> func);

        /**
         * Starts this service.
         * 
         * @throws std::logic_error in case this service is already running
         */
        void Startup();

        /**
         * Shuts this service down.
         * 
         * In case this service is currently not running, calling this method
         * has no effect.
         */
        void Shutdown();

        /**
         * Pauses this service.
         */
        void Pause();

        /**
         * Returns whether this service has been paused.
         * 
         * @return `true` if this service is currently in paused-state.
         */
        bool IsPaused() const;

        /**
         * Returns whether this service has been started.
         * 
         * @eturn `true` if this service has been started
         */
        bool IsStarted() const {
            return GetStatus() == Status::Running 
                || GetStatus() == Status::Paused;
        }

        /**
         * Resumes this state from paused state.
         */
        void Resume();

    protected:

        /**
         * The base class calls this method on startup.
         * 
         * Derived can override this method for initialization purposes.
         */
        virtual void OnStartup() {}

        /**
         * This base class calls this method on shutdown.
         * 
         * Derived can override this method for de-initialization purposes,
         * e.g., releasing resources, etc.
         */
        virtual void OnShutdown() {}

        /**
         * Called when this service switches from started to paused state.
         */
        virtual void OnPaused() {}

        /**
         * Called when this service switches from paused to running.
         */
        virtual void OnResumed() {}

    private:
        /** The name of this service. */
        std::string name;

        /** The status of this serviced. */
        Status status;

        /** Startup hooks. */
        std::vector<std::function<void (void)>> startupHooks;

        /** Shutdown hooks. */
        std::vector<std::function<void (void)>> shutdownHooks;

        /** Pause hooks. */
        std::vector<std::function<void (void)>> pauseHooks;

        /** Resume hooks. */
        std::vector<std::function<void (void)>> resumeHooks;
    };

    /**
     * Base class for new (custom) services.
     */
    class BaseService : virtual public Service {
    public:

        BaseService(const std::string name = Service::DEFAULT_NAME)
            : Service(name)
        {
            // Intentionally left empty
        }

        /** Virtual destructor. */
        virtual ~BaseService() {}

    };

} // end of namespace