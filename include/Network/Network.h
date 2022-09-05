/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/Socket.h"

// Local includes
#include "Network/IpMode.h"

// C++ Standard Library includes
#include <stdint.h>
#include <memory>
#include <string>

namespace astu {

    // Forward declaration
    class NetworkImpl;

    /**
     * Wrapper for operating system dependent network operations.
     *
     * The current interface focuses on UDP-based non-blocking sockets.
     */
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
         * Returns the currently set IP mode.
         *
         * @return the current IP mode
         */
        IpMode GetIpMode() const;

        /**
         * Creates, registers and returns a handle to an IP socket address.
         *
         * The host name is a string with either an ip address or a domain name.
         * e.g "192.168.0.17" or "platypus.com".
         *
         * @param host  the host name
         * @param port  the port number
         * @return the handle to the ip socket address
         * @throws std::runtime_error in case the socket address could not be created
         */
        int CreateAddressHandle(const std::string& host, uint16_t port);

        /**
         * Tests whether a handle for a socket address already exists.
         *
         * @param host the host name
         * @param port the port number
         * @return 'true' if a handle for the specified socket address exists
         * @throws std::runtime_error in case the socket address could not be
         *  translated into an internal address structure use for comparison
         */
        bool HasAddressHandle(const std::string& host, uint16_t port) const;

        /**
         * Returns the address handle of a socket address.
         *
         * The host name is a string with either an ip address or a domain name.
         * e.g "192.168.0.17" or "platypus.com".
         *
         * @param host  the host name
         * @param port  the port number
         * @return the address handle
         * @throws std::logic_error in case the specified socket address is unknown
         * @throws std::runtime_error in case the socket address could not be
         *  translated into an internal address structure use for comparison
         */
        int GetAddressHandle(const std::string& host, uint16_t port);

        /**
         * Convenient method used to get a address handle for the specified socket address.
         * In case the specified socket address has not yet been associated with an address handle,
         * a new address handle is created.
         *
         * The host name is a string with either an ip address or a domain name.
         * e.g "192.168.0.17" or "platypus.com".
         *
         * @param host  the host name
         * @param port  the port number
         * @return the address handle
         * @throws std::runtime_error in case the socket address could not be
         *  translated into an internal address structure use for comparison
         */
        int GetOrCreateAddressHandle(const std::string& host, uint16_t port);

        /**
         * Creates a non-blocking UDP socket.
         *
         * If zero is specified as port number, a port number will be automatically assigned by the
         * operating system. Zero as port number is the usual parameter for client applications.
         *
         * @param port  the port number used to bind the socket
         * @return the newly created UDP socket
         * @throws std::runtime_error in case of an error
         */
        std::unique_ptr<Socket> CreateUdpSocket(uint16_t port = 0);

        /**
         * Creates a UDP socket.
         *
         * This method requires a local host address. This method is useful in
         * case the local machine has multiple address and/or network adapters.
         * Do not use remote hosts for the host parameter.
         *
         * @param host  the local host used to bind the socket
         * @param port  the port number used to bind the socket
         * @return the newly created socket
         * @throws std::runtime_error in case of an error
         */
        std::unique_ptr<Socket> CreateUdpSocket(const std::string &host, uint16_t port);

        /**
         * Return a human readable from of a socket address.
         *
         * @param hAddr the handle of the socket address
         * @return a string representation of the socket address
         * @throws std::logic_error if the socket handle is invalid
         */
        std::string GetAddressString(int hAddr) const;

    private:
        /** Pimpl-idiom, pointer to the real implementation. */
        std::shared_ptr<NetworkImpl> nwImpl;
    };

} // end of namespace
