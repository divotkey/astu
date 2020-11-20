/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
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
     * Service manager used to administer essential game-wide services.
     * This implementation realizes the service manager as a singleton.
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

}
