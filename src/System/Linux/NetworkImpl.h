/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>

namespace astu {

    // Forward declaration
    class UdpSocket;

    /**
     * Operating system specific implementation of the network module.
     */
    class NetworkImpl {
    public:

        /**
         * Constructor.
         */
        NetworkImpl();

        /**
         * Destructor.
         */
        ~NetworkImpl();

        std::unique_ptr<UdpSocket> CreateUdpSocket(uint16_t port = 0);

    private:
    };

} // end of namespace

