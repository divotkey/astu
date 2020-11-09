/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>

namespace astu {

    // Forward declaration.
    class ServiceManager;

    /**
     * Interface for services.
     */
    class IService {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~IService() {}

        /**
         * Returns the name of this service.
         * 
         * @return this service's name
         */
        virtual const std::string & GetName() const = 0;

        /**
         * Starts this service.
         * 
         * @throws std::logic_error in case this service is already running
         */
        virtual void Startup() = 0;

        /**
         * Stops this service.
         * 
         * In case this service is currently not running, calling this method
         * has no effect.
         */
        virtual void Shutdown() = 0;

        /**
         * Returns `true` if this service is running.
         * 
         * @return `true` if this service is running
         */
        virtual bool IsRunning() const = 0;
    };

    class BaseService : public IService {
    public:

        /** Default name for services. */
        static const std::string & DEFAULT_NAME;

        /**
         * Constructor.
         */
        BaseService(const std::string & name = DEFAULT_NAME);

        // Inherited via IService
        virtual const std::string & GetName() const final override;
        virtual void Startup() override;
        virtual void Shutdown() override;
        virtual bool IsRunning() const override;

    protected:

        /**
         * Called by this base class on startup.
         */
        virtual void OnStartup() {}

        /**
         * Called by this base class on shutdown.
         */
        virtual void OnShutdown() {}


        ServiceManager & GetSM();



    private:
        /** The name of this service. */
        std::string name;
        
        /** Whether this service is currently running. */
        bool running;

    };

    } // end of namespace