/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include "ServiceManager.h"

namespace astu {

    /**
     * Interface for services.
     * 
     * @ingroup srv_group
     */
    class IService : public std::enable_shared_from_this<IService> {
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

        template<typename T>
        std::shared_ptr<T> shared_as()
        {
            auto result = std::dynamic_pointer_cast<T>(shared_from_this());
            if (result == nullptr) {
                throw std::logic_error("Cannot cast to this type");
            }
            return result;
        }
    };

    /**
     * Implmenents basic functionality of a service.
     * 
     * This base class can be used to create services with basic
     * functionlality e.g, having a name and keeping track
     * if its currently running or not.
     * 
     * @ingroup srv_group
     */
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
         * 
         * Derived classes should override this method rather than
         * `Startup()`. The `Startup()` method is maintaining 
         * the running-state of this service.
         */
        virtual void OnStartup() {}

        /**
         * Called by this base class on shutdown.
         * 
         * Derived classes should override this method rather than
         * `Shutdown()`. The `Shutdown()` method is maintaining 
         * the running-state of this service.
         */
        virtual void OnShutdown() {}

        /**
         * Returns the service manager.
         * 
         * This method is right now just a convenience method 
         * to get access tot the service manager. However future
         * version of this service module might make use of 
         * multiple different service manages instead of using
         * a singleton. In this case this method becomes a
         * requirement.
         * 
         * @return the service manager
         */
        ServiceManager & GetSM();

    private:
        /** The name of this service. */
        std::string name;
        
        /** Whether this service is currently running. */
        bool running;
    };

    } // end of namespace