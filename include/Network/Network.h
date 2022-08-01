/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/IUdpSocket.h"

// C++ Standard Library includes
#include <memory>

namespace astu {

    // Forward declaration
    class NetworkImpl;

    class Network {
    public:

        /** Constructor. */
        Network();

        /** Destructor. */
        ~Network();

        /**
         * Creates a UDP socket.
         *
         * If zero is specified for the port number, the port number will
         * be automatically chosen.
         *
         * @param port  the port number used to bind the socket
         * @return the newly created port
         * @throws std::runtime_error in case of an error
         */
        std::unique_ptr<IUdpSocket> CreateUdpSocket(uint16_t port);

    private:
        /** Impl-idiom, pointer to the real implementation. */
        std::unique_ptr<NetworkImpl> nwImpl;
    };

} // end of namespace
