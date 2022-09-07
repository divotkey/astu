/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Logging/ConsoleLoggingService.h"

// C++ Standard Library includes
#include <iostream>

using namespace std;

namespace astu {

    const std::string ConsoleLoggingService::LOG_LEVELS[]
        = {"Fatal", "Error", "Warning", "Info", "Config", "Debug", "Verbose"};

    ConsoleLoggingService::ConsoleLoggingService()
        : Service("Console Logging Service")
    {
        // Intentionally left empty.
    }

    void ConsoleLoggingService::Log(LogLevel level, const string &tag, const std::string message) const
    {
        if (static_cast<int>(level) > static_cast<int>(GetMaxLevel()))
            return;

        cout << LOG_LEVELS[static_cast<int>(level)] << " [" << tag << "] " << message << endl;
    }

} // end of namespace