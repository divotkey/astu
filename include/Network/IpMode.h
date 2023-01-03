/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * Describes the allowed IP address versions.
     */
    enum class IpMode {
        /** Uses only IP v4 addresses. */
        IPv4,

        /** Uses only IP v6 addresses. */
        IPv6,

        /** Uses only IP v6 and v4 addresses. */
        IPv4_And_IPv6
    };

} // end of namespace
