/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

namespace astu {

    // Forward declaration.
    class IService;

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
         * Returns the one and only instnace of the service manager.
         * 
         * @return the one and only service managanger
         */
        static ServiceManager & GetInstance();

        /**
         * Adds a service to this manager.
         * 
         * @param service   the service to add
         * @throws std::logic_error in case the service has already been added.
         */
        void AddService(std::shared_ptr<IService> service);

        /**
         * Removes the specified service.
         * 
         * @param service the service to remove
         */
        void RemoveService(std::shared_ptr<IService> service);

        /**
         * Tests whether the specified services has already been added.
         * 
         * @param service   the service to test
         * @return `true` in case the service has already been added
         */
        bool HasService(std::shared_ptr<IService> service) const;

        /**
         * Starts up all servides.
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
         * Returns a service of a certain type.
         * 
         * **Example**
         * 
         * ```
         * // Fetch reference to the one and only service manager.
         * auto & sm = ServiceManager::GetInstance();
         * 
         * // Fetch reference to a service whith implements a certain interface,
         * // in this case the ITimeManager interface.
         * auto & timeSrv = sm.GetService<ITimeManager>();
         * 
         * // Use the time service.
         * double dt = timeSrv.GetElapsedTime();
         * ```
         * 
         * 
         * @tparam T    the type of service to look for
         * @return the requested service
         * @throws std::logic_error in case the service could not be found
         */
        template <typename T> 
        T & GetService()
        {
            auto srv = FindService<T>();
            if (srv == nullptr) {
                throw std::logic_error(std::string("Service not found, type is '") 
                    + typeid(T).name() + "'");
            }

            return *srv;
        }

        /**
         * Searches for a service of a certain type.
         * 
         * **Example**
         * 
         * ```
         * // Fetch reference to the one and only service manager.
         * auto & sm = ServiceManager::GetInstance();
         * 
         * // Get (smart) pointer to a service whith implements a certain interface,
         * // in this case the ITimeManager interface.
         * auto pTimeSrv = sm.FindService<ITimeManager>();
         * 
         * // Verify that the requested service actually exists
         * if (pTimeSrv == nullptr) {
         *   ReportError("ITimeService required");
         *   return;
         * }
         * 
         * // Use the time service.
         * double dt = timeSrv->GetElapsedTime();
         * ```
         * 
         * @tparam T    the type of service to look for
         * @return the requested service of `nullptr` if no appropriate service could be found
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

            return nullptr;
        }

    private:
        /** The one and only instance of a service manager. */
        static ServiceManager * theOneAndOnly;

        /** The services administered by this manager. */
        std::vector<std::shared_ptr<IService>> services;

        /** Indicates that the services are already running. */
        bool running;

        /**
         * Constructor.
         */
        ServiceManager();

        /**
         * Copy-constructor (deleted).
         * 
         * Prevents creating implicit copies of this sinleton.
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