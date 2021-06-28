/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

// Service Macros
#define ASTU_SERVICE_MANAGER() astu::ServiceManager::GetInstance()
#define ASTU_SERVICE(type) (*ASTU_SERVICE_MANAGER().FindService<type>())
#define ASTU_GET_SERVICE(type) ASTU_SERVICE_MANAGER().FindService<type>()
#define ASTU_GET_SERVICE_OR_DEFAULT(type, b) ASTU_SERVICE_MANAGER().FindService<type>(b)
#define ASTU_GET_SERVICE_OR_NULL(type) ASTU_SERVICE_MANAGER().FindService<type>(nullptr)
#define ASTU_HAS_SERVICE(type) (ASTU_GET_SERVICE_OR_NULL(type) != nullptr)

#define ASTU_REMOVE_ALL_SERVICES() ASTU_SERVICE_MANAGER().RemoveAllServices()
#define ASTU_STARTUP_SERVICES() ASTU_SERVICE_MANAGER().StartupAll()
#define ASTU_SHUTDOWN_SERVICES() ASTU_SERVICE_MANAGER().ShutdownAll()

#define ASTU_ADD_SERVICE(srv) ASTU_SERVICE_MANAGER().AddService(srv)
#define ASTU_CREATE_AND_ADD_SERVICE(srvType, ...) ASTU_ADD_SERVICE(std::make_shared<srvType>(__VA_ARGS__) )

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
         * @throws std::logic_error in case the service has already been added.
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
         * // Get (smart) pointer to a service which implements a certain interface,
         * // in this case the ITimeManager interface.
         * ITimeManager *pTimeSrv = sm.FindService<ITimeManager>();
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