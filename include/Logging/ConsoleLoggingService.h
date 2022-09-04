/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/LoggingService.h"

namespace astu {

    /**
     * Implements the LoggingService class and logs to standard output.
     *
     * @ingroup logging_group
     */
    class ConsoleLoggingService
        : public virtual Service
        , public LoggingService
    {
    public:

        ConsoleLoggingService();

        void Log(LogLevel level, const std::string &tag, const std::string message) const override;

    private:
        static const std::string LOG_LEVELS[];

    };



} // end of namespace