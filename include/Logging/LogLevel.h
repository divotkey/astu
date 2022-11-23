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

    /**
     * Defines the hierarchy of log levels.
     *
     * @ingroup logging_group
     */
    enum class LogLevel {
        Fatal,
        Err,
        Warn,
        Info,
        Config,
        Debug,
        Verbose
    };

    /**
     * A utility class providing log level related static utility methods.
     *
     * @ingroup logging_group
     */
    class LogLevelUtils final  {
    public:

        /**
         * Deleted constructor -> no instances of this class allowed.
         */
        LogLevelUtils() = delete;
        LogLevelUtils(const LogLevelUtils&) = delete;
        LogLevelUtils(LogLevelUtils&&) = delete;

        /**
         * Converts strings to logging levels.
         * The conversion is case insensitive.
         *
         * @param level the logging level as string
         * @return the corresponding log level
         * @throws std::logic_error in case the string does not math a logging level
         */
        static LogLevel StringToLevel(const std::string &level);
    };

} // end of namespace