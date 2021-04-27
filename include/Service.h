/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>
#include "ServiceManager.h"

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

    private:
        /** The name of this service. */
        std::string name;

        /** The status of this serviced. */
        Status status;

        /** Startup hooks. */
        std::vector<std::function<void (void)>> startupHooks;

        /** Shutdown hooks. */
        std::vector<std::function<void (void)>> shutdownHooks;
    };

    /**
     * Base class for new (custom) services.
     */
    class BaseService : virtual public Service {
    public:

        /** Virtual destructor. */
        virtual ~BaseService() {}

    };

    // /**
    //  * Interface for services.
    //  * 
    //  * @ingroup srv_group
    //  */
    // class IService : public std::enable_shared_from_this<IService> {
    // public:

    //     /**
    //      * Virtual destructor.
    //      */
    //     virtual ~IService() {}

    //     /**
    //      * Returns the name of this service.
    //      * 
    //      * @return this service's name
    //      */
    //     virtual const std::string & GetName() const = 0;

    //     /**
    //      * Starts this service.
    //      * 
    //      * @throws std::logic_error in case this service is already running
    //      */
    //     virtual void Startup() = 0;

    //     /**
    //      * Stops this service.
    //      * 
    //      * In case this service is currently not running, calling this method
    //      * has no effect.
    //      */
    //     virtual void Shutdown() = 0;

    //     /**
    //      * Returns `true` if this service is running.
    //      * 
    //      * @return `true` if this service is running
    //      */
    //     virtual bool IsRunning() const = 0;

    //     template<typename T>
    //     std::shared_ptr<T> shared_as()
    //     {
    //         auto result = std::dynamic_pointer_cast<T>(shared_from_this());
    //         if (result == nullptr) {
    //             throw std::logic_error(std::string("Unable to cast this service to type ") 
    //                 + typeid(T).name());
    //         }
    //         return result;
    //     }
    // };

    // /**
    //  * Implements basic functionality of a service.
    //  * 
    //  * This base class can be used to create services with basic
    //  * functionality e.g, having a name and keeping track
    //  * if its currently running or not.
    //  * 
    //  * @ingroup srv_group
    //  */
    // class BaseService : public IService {
    // public:

    //     /** Default name for services. */
    //     static const std::string & DEFAULT_NAME;

    //     /**
    //      * Constructor.
    //      */
    //     BaseService(const std::string & name = DEFAULT_NAME);

    //     // Experimental
    //     void AddStartupHook(std::function<void (void)> func);
    //     void AddShutdownHook(std::function<void (void)> func);

    //     // Inherited via IService
    //     virtual const std::string & GetName() const final override;
    //     virtual void Startup() override;
    //     virtual void Shutdown() override;
    //     virtual bool IsRunning() const final override;

    // protected:

    //     /**
    //      * Called by this base class on startup.
    //      * 
    //      * Derived classes should override this method rather than
    //      * `Startup()`. The `Startup()` method is maintaining 
    //      * the running-state of this service.
    //      */
    //     virtual void OnStartup() {}

    //     /**
    //      * Called by this base class on shutdown.
    //      * 
    //      * Derived classes should override this method rather than
    //      * `Shutdown()`. The `Shutdown()` method is maintaining 
    //      * the running-state of this service.
    //      */
    //     virtual void OnShutdown() {}

    //     /**
    //      * Returns the service manager.
    //      * 
    //      * This method is right now just a convenience method 
    //      * to get access tot the service manager. However future
    //      * version of this service module might make use of 
    //      * multiple different service manages instead of using
    //      * a singleton. In this case this method becomes a
    //      * requirement.
    //      * 
    //      * @return the service manager
    //      */
    //     ServiceManager & GetSM();

    // private:
    //     /** The name of this service. */
    //     std::string name;
        
    //     /** Whether this service is currently running. */
    //     bool running;

    //     /** Startup hooks. */
    //     std::vector<std::function<void (void)>> startupHooks;

    //     /** Shutdown hooks. */
    //     std::vector<std::function<void (void)>> shutdownHooks;
    // };

    } // end of namespace