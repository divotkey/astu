/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/SignalService.h"

// C++ Standard Library includes
#include <string>
#include <chrono>
#include <map>

namespace astu {

class ConsoleApplication : public ISignalListener<std::string> {
    public:

        /** Predefined string property name defining the application name. */
        static const std::string APP_NAME_PROP;

        /** Predefined string property name defining the application version string. */
        static const std::string APP_VERSION_PROP;

        /** Predefined string property name defining the application copyright year. */
        static const std::string COPYRIGHT_YEAR_PROP;

        /** Predefined string property name defining the application copyright holder. */
        static const std::string COPYRIGHT_HOLDER_PROP;

        /** Predefined string property name defining whether application info should be printed. */
        static const std::string SHOW_APP_INFO_PROP;

        /**
         * Constructor.
         */
        ConsoleApplication();

        /**
         * Virtual destructor.
         */
        virtual ~ConsoleApplication() {}

        /**
         * Convenient method to retrieve the version information as string.
         *
         * @return the version information of this application
         */
        const std::string& GetVersionString() const {
            return GetStringProperty(APP_VERSION_PROP);
        }

        /**
         * Convenient method to set the version information string for this application.
         *
         * @param version   the version information string
         */
        void SetVersionString(const std::string& version) {
            SetStringProperty(APP_VERSION_PROP, version);
        }

        /**
         * Convenient method to retrieve the name of this application.
         *
         * @return the application name
         */
        const std::string& GetApplicationName() const {
            return GetStringProperty(APP_NAME_PROP);
        }

        /**
         * Convenient method to set the name of this application.
         *
         * @param name  the application name
         */
        void SetApplicationName(const std::string& name) {
            SetStringProperty(APP_NAME_PROP, name);
        }

        /**
         * Convenient method to retrieve the copyright holder of this application.
         *
         * @return the copyright holder
         */
        const std::string& GetCopyrightHolder() const {
            return GetStringProperty(COPYRIGHT_HOLDER_PROP);
        }

        /**
         * Convenient method to set information about the copyright holder of this application.
         *
         * @param holder    the copyright year
         */
        void SetCopyrightHolder(const std::string& holder) {
            SetStringProperty(COPYRIGHT_HOLDER_PROP, holder);
        }

        /**
         * Convenient method retrieve the copyright year string of this application.
         *
         * @return the copyright holder
         */
        const std::string& GetCopyrightYear() const {
            return GetStringProperty(COPYRIGHT_YEAR_PROP);
        }

        /**
         * Convenient method to set the information about the copyright year of this application.
         *
         * @param holder    the copyright year
         */
        void SetCopyrightYear(const std::string& year) {
            SetStringProperty(COPYRIGHT_YEAR_PROP, year);
        }

        /**
         * Convenient method to assemble application information string (name, version etc).
         *
         * @return a string containing application information
         */
        std::string GetInfoString() const;

        /**
         * Sets the updates per seconds this application should do.
         *
         * @param ups updates per second
         */
        void SetUpdatesPerSecond(double ups);

        /**
         * Returns the updates per second this application is supposed to do.
         *
         * @return updates per second
         */
        double GetUpdatesPerSecond() const;

        /**
         * Starts all services and runs the main loop of this application.
         *
         * @return zero if the application has terminated successfully
         */
        int Run();

        /**
         * Returns the value of a string property.
         *
         * @param name  the name of the string property.
         * @return the requested string property
         * @throws std::logic_error in case the string property is unknown
         */
        const std::string& GetStringProperty(const std::string& name) const;

        /**
         * Returns the value of a string property.
         *
         * @param name  the name of the string property.
         * @param defaultValue  the default value in case the property has not been set
         * @return the requested string property
         * @throws std::logic_error in case the string property is unknown
         */
        const std::string& GetStringProperty(const std::string& name, const std::string& defaultValue) const;

        /**
         * Returns whether a string property has been set.
         *
         * @param name  the name of the property
         * @return `true` if the string property has been set
         */
        bool HasStringProperty(const std::string& name) const;

        /**
         * Sets the value of a string property.
         *
         * @param name      the name of the string property
         * @param value     the value of the string property
         */
        void SetStringProperty(const std::string& name, const std::string& value);

        /**
         * Tests whether a certain flag has been set.
         * If a flag is unknown, this method returns false
         *
         * @param name          the name of the flag
         * @param defaultValue  the default value to be returned if the flag is unknown
         * @return the value of the flag
         */
        bool IsFlagSet(const std::string& name, bool defaultValue) const;

        /**
         * Tests whether a certain flag has been set.
         *
         * @param name          the name of the flag
         * @return the value of the flag
         * @throws std::logic_error in case the flag is unknown
         */
        bool IsFlagSet(const std::string& name) const;

        /**
         * Sets or clears a flag.
         *
         * @param name      the name of the flag
         * @param value     the value of the flag (true or false)
         */
        void SetFlag(const std::string& name, bool value = true);

        /**
         * Returns whether a flag has been set.
         *
         * @param name  the name of the flag
         * @return `true` if the flag has been set
         */
        bool HasFlag(const std::string& name) const;

        /**
         * Schedules the termination of this application.
         */
        void Terminate();

        // Inherited via ISignalListener<std::string>
        virtual bool OnSignal(const std::string & signal) override;

protected:

        /**
         * Configures services according to application specific settings.
         */
        virtual void ConfigureApplication();

        /**
         * Releases resources, de-registers as listener etc.
         */
        virtual void Cleanup();

    private:
        /** Property table for string values. */
        std::map<std::string, std::string> stringProperties;

        /** Property table for boolean values. */
        std::map<std::string, bool> boolProperties;

        /** The target delay per cycle to reach certain targeted updates per second. */
        std::chrono::nanoseconds targetDelay;

        /** Indicates that this application has been terminated ans scheduled to stop running. */
        bool terminated;

        /**
         * Adds required core services.
         */
        void AddCoreServices();

        /**
         * Prints version information to the terminal.
         */
        void PrintVersionInfo();

        /**
         * Runs the application loop, maintaining a certain update rate.
         *
         * This method governs the update rate by using C++ standard sleep method.
         * This is not as precise as governing the update rate with busy wait CPU cycles,
         * but will relieve the CPU and lower the power consumption.
         */
        void LoopWithSpinLock();

        void WaitWithSpinLock(std::chrono::nanoseconds duration);
    };

} // end of namespace



