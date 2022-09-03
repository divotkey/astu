/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * Defines the hierarchy of log levels.
     *
     * @ingroup loggin_group
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

} // end of namespace