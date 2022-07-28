/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>

namespace astu {

    class ConsoleApplication {
    public:

        /**
         * Constructor.
         */
        ConsoleApplication();

        /**
         * Virtual destructor.
         */
        virtual ~ConsoleApplication() {}


        /**
         * Returns version information as string.
         *
         * @return the version information of this application
         */
        const std::string& GetVersionString() const;

        /**
         * Sets the version information string for this application.
         *
         * @param version   the version information string
         */
        void SetVersionString(const std::string& version);

        /**
         * Returns name of this application.
         *
         * @return the application name
         */
        const std::string& GetApplicationName() const;

        /**
         * Sets the name of this application.
         *
         * @param name  the application name
         */
        void SetApplicationName(const std::string& name);

        /**
         * Returns a application information e.g., name, version etc.
         *
         * @return a string containing application information
         */
        std::string GetInfoString() const;

        /**
         * Starts all services and runs the main loop of this application.
         *
         * @return zero if the application has terminated successfully
         */
        int Run();

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

        /** The name of this application. */
        std::string appName;

        /** Contains version information about this application. */
        std::string versionString;

        /**
         * Adds required core services.
         */
        void AddCoreServices();

        /**
         * Prints version information to the terminal.
         */
        void PrintVersionInfo();
    };

} // end of namespace



