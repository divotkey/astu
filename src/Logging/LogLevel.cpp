/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Logging/LogLevel.h"
#include "Util/StringUtils.h"

// C++ Standard Library includes
#include <string>
#include <stdexcept>

using namespace std;

namespace astu {

    LogLevel LogLevelUtils::StringToLevel(const string &level)
    {
        string lvl = level;
        StringUtils::toLowerCase(lvl);

        if (lvl == "fatal")
            return LogLevel::Fatal;
        else if (lvl == "err")
            return LogLevel::Err;
        else if (lvl == "warn")
            return LogLevel::Warn;
        else if (lvl == "info")
            return LogLevel::Config;
        else if (lvl == "debug")
            return LogLevel::Debug;
        else if (lvl == "verbose")
            return LogLevel::Verbose;

        throw std::logic_error("Unable to convert string to logging level: '" + level + "'");
    }


} // end of namespace