/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Logging/LogLevel.h"
#include "Service.h"

// C++ Standard Library includes
#include <string>

namespace astu {

    /**
     * This abstract base class used for UDP network communication.
     * 
     * @ingroup srv_group
     */
    class LoggingService {
    public:

        LoggingService() : maxLevel(LogLevel::Debug) {};

        /**
         * Virtual destructor.
         */
        virtual ~LoggingService() {}

        /**
         * Emits a logging entry.
         *
         * @param level     the logging level
         * @param tag       a tag used to identify the logging source or source group
         * @param message   the actual logging message
         */
        virtual void Log(LogLevel level, const std::string& tag, const std::string message) const = 0;

        void LogFatal(const std::string& tag, const std::string message) const {
            Log(LogLevel::Fatal, tag, message);
        }

        void LogError(const std::string& tag, const std::string message) const {
            Log(LogLevel::Err, tag, message);
        }

        void LogInfo(const std::string& tag, const std::string message) const {
            Log(LogLevel::Info, tag, message);
        }

        void LogWarning(const std::string& tag, const std::string message) const {
            Log(LogLevel::Warn, tag, message);
        }

        void LogConfig(const std::string& tag, const std::string message) const {
            Log(LogLevel::Config, tag, message);
        }

        void LogDebug(const std::string& tag, const std::string message) const {
            Log(LogLevel::Debug, tag, message);
        }

        void LogVerbose(const std::string& tag, const std::string message) const {
            Log(LogLevel::Verbose, tag, message);
        }

        void SetMaxLevel(LogLevel level) {
            maxLevel = level;
        }

        LogLevel GetMaxLevel() const {
            return maxLevel;
        }

    private:
        LogLevel maxLevel;
    };

    /**
     * Services can derive from this class to have easy access to logging service.
     *
     * @ingroup srv_group
     */
    class LoggingClient : virtual Service {
    public:

        /**
         * Constructor.
         */
        LoggingClient() {
            AddStartupHook([this]() { loggingSrv = ASTU_GET_SERVICE(LoggingService); });
            AddShutdownHook([this]() { loggingSrv = nullptr; });
        }

        /** Virtual destructor. */
        virtual ~LoggingClient() { }

    protected:

        void LogFatal(const std::string& tag, const std::string message) const {
            loggingSrv->Log(LogLevel::Fatal, tag, message);
        }

        void LogError(const std::string& tag, const std::string message) const {
            loggingSrv->Log(LogLevel::Err, tag, message);
        }

        void LogInfo(const std::string& tag, const std::string message) const {
            loggingSrv->Log(LogLevel::Info, tag, message);
        }

        void LogWarning(const std::string& tag, const std::string message) const {
            loggingSrv->Log(LogLevel::Warn, tag, message);
        }

        void LogConfig(const std::string& tag, const std::string message) const {
            loggingSrv->Log(LogLevel::Config, tag, message);
        }

        void LogDebug(const std::string& tag, const std::string message) const {
            loggingSrv->Log(LogLevel::Debug, tag, message);
        }

        void LogVerbose(const std::string& tag, const std::string message) const {
            loggingSrv->Log(LogLevel::Verbose, tag, message);
        }

    private:
        /** The logging service used by this class. */
        std::shared_ptr<LoggingService> loggingSrv;
    };


} // end of namespace