/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Linux API includes
#include <netinet/in.h>

// C++ Standard Library includes
#include <stdint.h>

namespace astu {

    class Inet4SocketAddress {
    public:

        /**
         * Constructor.
         *
         * @param port  the port number, 0 to get a system assigned port
         */
        Inet4SocketAddress(uint16_t port = 0);

    private:
        /** The actual IPv4 socket address structure. */
        struct sockaddr_in addr;
    };

} // end of namespace
