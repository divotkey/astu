/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/ISocket.h"
#include "Network/IInetSocketAddress.h"

// Local includes
#include "Network/IpMode.h"

// C++ Standard Library includes
#include <stdint.h>
#include <memory>
#include <string>

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
         * Specifies the IP mode.
         *
         * @param mode the IP mode
         * @return reference to this class for method chaining
         */
        void SetIpMode(IpMode mode);

        /**
         * Creates a UDP socket.
         *
         * @param port  the port number used to bind the socket
         * @return the newly created socket
         * @throws std::runtime_error in case of an error
         */
        std::unique_ptr<ISocket> CreateUdpSocket(uint16_t port = 0);

        /**
         * Creates a UDP socket.
         *
         * This method requires a local host address to work. Do not use
         * remote hosts for the host parameter.
         *
         * @param host  the local host used to bind the socket
         * @param port  the port number used to bind the socket
         * @return the newly created socket
         * @throws std::runtime_error in case of an error
         */
        std::unique_ptr<ISocket> CreateUdpSocket(const std::string &host, uint16_t port);

        std::unique_ptr<IInetSocketAddress> CreateUdpSocketAddress(const std::string & host, uint16_t port);

    private:
        /** Impl-idiom, pointer to the real implementation. */
        std::shared_ptr<NetworkImpl> nwImpl;
    };

} // end of namespace
