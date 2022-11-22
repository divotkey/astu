/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

/////////////////////////////////////////////////
/////// Service Macros
/////////////////////////////////////////////////

/**
 * Returns the instance of the service manager.
 * 
 * **Example**
 * 
 * ```
 * ASTU_SERVICE_MANAGER.RemoveAllServices();
 * ```
 * 
 * @return the service manager instance
 * @ingroup srv_group
 */
#define ASTU_SERVICE_MANAGER() astu::ServiceManager::GetInstance()

/**
 * Returns a reference to a service.
 * 
 * **Example**
 * 
 * ```
 * double dt = ASTU_SERVICE(TimeService).GetElapsedTime();
 * ```
 * 
 * @return the requested service
 * @throws std::logic_error in case the requested service is unknown
 * @ingroup srv_group
 */
#define ASTU_SERVICE(type) (*ASTU_SERVICE_MANAGER().FindService<type>())

/**
 * Returns a (smart-) pointer to a service.
 * 
 * Getting a pointer to a service is used to store this pointer for later use.
 * Keeping a pointer of a service is preferable if a service is accessed several
 * times in sequence or, e.g., in each cycle of the game loop. 
 * 
 * **Example**
 * 
 * ```
 * std::shared_ptr<TimeService> timeSrv = ASTU_GET_SERVICE(TimeService);
 * double dt = timeSrv->GetElapsedTime();
 * ```
 * 
 * @return the requested service
 * @throws std::logic_error in case the requested service is unknown
 * @ingroup srv_group
 */
#define ASTU_GET_SERVICE(type) ASTU_SERVICE_MANAGER().FindService<type>()

/**
 * Returns a (smart-) pointer to a service.
 * 
 * This macro will return a the requested service or the specified default
 * service in case the service does not exist.
 *   
 * @return the requested service or the specified default service
 * @ingroup srv_group
 */

#define ASTU_GET_SERVICE_OR_DEFAULT(type, b) ASTU_SERVICE_MANAGER().FindService<type>(b)

/**
 * Returns a (smart-) pointer to a service.
 * 
 * This macro will return a the requested service or `nullptr` if the service
 * does not exist. In case the existence of a service is optional, it is faster
 * to query the pointer to a service and then if this pointer is `nullptr` or
 * not, than using calling `ASTU_HAS_SERVCIE` and then calling
 * `ASTU_GET_SERVICE`.
 *  
 * **Example**
 * 
 * ```
 * std::shared_ptr<SignalService<int>> intSignals = ASTU_GET_SERVICE_OR_NULL(SignalService<int>);
 * 
 * if (intSignals) {
 *   intSignals->QueueSignal(42);
 *
 * }
 * ```
 * 
 * @return the requested service or `nullptr` if the service does not exist
 * @ingroup srv_group
 */
#define ASTU_GET_SERVICE_OR_NULL(type) ASTU_SERVICE_MANAGER().FindService<type>(nullptr)

/**
 * Returns whether a certain service exists
 * 
 * **Example**
 * 
 * ```
 * std::shared_ptr<SignalService<int>> intSignals = ASTU_GET_SERVICE_OR_NULL(SignalService<int>);
 * 
 * if (ASTU_HAS_SERVICE(SignalService<int>)) {
 *   ASTU_SERVICE(SignalService<int>).QueueSignal(42);
 * }
 * ```
 * 
 * @return `true` if the requested service exists
 * @ingroup srv_group
 */
#define ASTU_HAS_SERVICE(type) (ASTU_GET_SERVICE_OR_NULL(type) != nullptr)

/**
 * Removes all previously added services.
 * 
 * @ingroup srv_group
 */
#define ASTU_REMOVE_ALL_SERVICES() ASTU_SERVICE_MANAGER().RemoveAllServices()

/**
 * Starts up all services.
 * 
 * @ingroup srv_group
 */
#define ASTU_STARTUP_SERVICES() ASTU_SERVICE_MANAGER().StartupAll()

/**
 * Shuts down all services.
 * 
 * @ingroup srv_group
 */
#define ASTU_SHUTDOWN_SERVICES() ASTU_SERVICE_MANAGER().ShutdownAll()

/**
 * Adds a new service to the service manager.
 * 
 * **Example**
 * 
 * ```
 * ASTU_ADD_SERVICE( std::make_shared<SignalService<int>>() );
 * ```
 * 
 * @param srv   the service to add
 * @throws std::runtime_error in case the service has already been added
 * 
 * @ingroup srv_group
 */
#define ASTU_ADD_SERVICE(srv) ASTU_SERVICE_MANAGER().AddService(srv)

/**
 * Adds creates a new service.
 * 
 * This macro will create a new dynamic instance of the specified service type.
 * 
 * **Example**
 * 
 * ```
 * auto newService = ASTU_CREATE_SERVICE( MyService );
 * ```
 * 
 * @param srvType   the type service to add
 * @param ...   the optional constructor parameters for the service
 * @throws std::runtime_error in case the service has already been added
 * 
 * @ingroup srv_group
 */

#define ASTU_CREATE_SERVICE(srvType, ...) std::make_shared<srvType>(__VA_ARGS__)

/**
 * Adds a new service to the service manager.
 * 
 * This macro will create a new dynamic instance of the specified service type
 * and adds it to the service manager.
 * 
 * **Example**
 * 
 * ```
 * ASTU_CREATE_AND_ADD_SERVICE( SignalService<int> );
 * ASTU_CREATE_AND_ADD_SERVICE( CameraControlService, Priority::HIGH ); * 
 * ```
 * 
 * @param srvType   the type service to add
 * @param ...   the optional constructor parameters for the service
 * @throws std::runtime_error in case the service has already been added
 * 
 * @ingroup srv_group
 */
#define ASTU_CREATE_AND_ADD_SERVICE(srvType, ...) if(!ASTU_HAS_SERVICE(srvType)) ASTU_ADD_SERVICE(std::make_shared<srvType>(__VA_ARGS__) )

#define ASTU_REMOVE_SERVICE(srvType) ASTU_SERVICE_MANAGER().RemoveService(ASTU_GET_SERVICE_OR_NULL(srvType));

namespace astu {

    // Forward declaration.
    class Service;

    /**
     * Service manager is used administer essential application-wide services.
     * 
     * This implementation realizes the service manager as a singleton.
     * 
     * @ingroup srv_group
     */
    class ServiceManager {
    public:

        /**
         * Returns the one and only instance of the service manager.
         * 
         * @return the one and only service manager
         */
        static ServiceManager & GetInstance();

        /**
         * Adds a service to this manager.
         * 
         * @param service   the service to add
         * @throws std::logic_error in case the service has already been added
         */
        void AddService(std::shared_ptr<Service> service);

        /**
         * Removes the specified service.
         * 
         * @param service the service to remove
         */
        void RemoveService(std::shared_ptr<Service> service);

		/**
		 * Removes all services.
		 */
		void RemoveAllServices();

        /**
         * Tests whether the specified services has already been added.
         * 
         * @param service   the service to test
         * @return `true` in case the service has already been added
         */
        bool HasService(std::shared_ptr<Service> service) const;

        /**
         * Starts up all services.
         */
        void StartupAll();

        /**
         * Shuts down all services.
         */
        void ShutdownAll();

        /**
         * Returns whether the services have been started.
         * 
         * @return `true` if the services have been started
         */
        bool IsRunning() const;

        /**
         * Searches for a service of a certain type.
         * 
         * **Example**
         * 
         * ```
         * // Fetch reference to the one and only service manager.
         * auto & sm = ServiceManager::GetInstance();
         * 
         * // Get a (smart-) pointer to a service which implements a certain
         * // interface, in this case the TimeService interface.
         * auto timeSrv = sm.FindService<TimeService>();
         * 
         * // Use the time service.
         * double dt = timeSrv->GetElapsedTime();
         * ```
         * 
         * @tparam T    the type of service to look for
         * @return the requested service 
         * @throws std::logic_error in case the service could not be found
         */
        template <typename T> 
        std::shared_ptr<T> FindService()
        {
            for (auto srv : services) {
                auto castedService = std::dynamic_pointer_cast<T>(srv);
                if (castedService != nullptr) {
                    return castedService;
                }
            }

            throw std::logic_error(std::string("No service of type '") 
                + typeid(T).name() + "' found");
        }

        /**
         * Searches for a service of a certain type.
         * 
         * @tparam T                the type of service to look for
         * @param defaultResult     the default value if no appropriate service could be found
         * @return the requested service or the specified default value
         */
        template <typename T> 
        std::shared_ptr<T> FindService(std::shared_ptr<T> defaultResult)
        {
            for (auto srv : services) {
                auto castedService = std::dynamic_pointer_cast<T>(srv);
                if (castedService != nullptr) {
                    return castedService;
                }
            }

            return defaultResult;
        }        

    private:
        /** The one and only instance of a service manager. */
        static ServiceManager * theOneAndOnly;

        /** The services administered by this manager. */
        std::vector<std::shared_ptr<Service>> services;

        /** Indicates that the services are already running. */
        bool running;

        /**
         * Constructor.
         */
        ServiceManager();

        /**
         * Copy-constructor (deleted).
         * 
         * Prevents creating implicit copies of this singleton.
         */ 
        ServiceManager(const ServiceManager& o) = delete;

        /**
         * Assignment operator (deleted).
         * 
         * Prevents creating explicit copies of this singleton.
         */
        ServiceManager& operator=(const ServiceManager&) = delete;
    };

} // end of namespace