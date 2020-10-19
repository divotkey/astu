/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>

namespace astu {

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
        virtual void startup() = 0;

        /**
         * Stops this service.
         * 
         * In case this service is currently not running, calling this method
         * has no effect.
         */
        virtual void shutdown() = 0;

        /**
         * Returns `true` if this service is running.
         * 
         * @return `true` if this service is running
         */
        virtual bool isRunning() const = 0;
    };

    class BaseService : public IService {
    public:

        /**
         * Constructor.
         */
        BaseService(const std::string & name = "NO NAME");

        // Inherited via IService
        virtual const std::string & GetName() const override;
        virtual void startup() override;
        virtual void shutdown() override;
        virtual bool isRunning() const override;

    protected:

        /**
         * Called by this base class on startup.
         */
        virtual void onStartup() {}

        /**
         * Called by this base class on shutdown.
         */
        virtual void onShutdown() {}

    private:
        /** The name of this service. */
        std::string name;
        
        /** Whether this service is currently running. */
        bool running;

    };

    } // end of namespace