/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Service/LoggingService.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// LoggingClient
    /////////////////////////////////////////////////

    void LoggingClient::Log(LogLevel level, const std::string &tag, const std::string message) const
    {
        if (IsStarted()) {
            loggingSrv->Log(level, tag, message);
        } else {
            preStartMessages.push_back(LogEntry(level, tag, message));
        }
    }

    void LoggingClient::FlushPreStartMessages()
    {
        for (const auto &entry : preStartMessages) {
            loggingSrv->Log(entry.level, entry.tag, entry.message);
        }

        preStartMessages.clear();
    }

} // end of namespace