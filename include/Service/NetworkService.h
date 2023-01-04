/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stdint.h>
#include <string>
#include <memory>

namespace astu {

    // Forward declaration
    class Socket;

    /**
     * This abstract base class used for UDP network communication.
     * 
     * @ingroup srv_group
     */
    class NetworkService {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~NetworkService() {}

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
        virtual int CreateAddressHandle(const std::string& host, uint16_t port) = 0;

        /**
         * Tests whether a handle for a socket address already exists.
         *
         * @param host the host name
         * @param port the port number
         * @return 'true' if a handle for the specified socket address exists
         * @throws std::runtime_error in case the socket address could not be
         *  translated into an internal address structure use for comparison
         */
        virtual bool HasAddressHandle(const std::string& host, uint16_t port) const = 0;

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
        virtual int GetAddressHandle(const std::string& host, uint16_t port) = 0;

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
        virtual int GetOrCreateAddressHandle(const std::string& host, uint16_t port) = 0;

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
        virtual std::unique_ptr<Socket> CreateUdpSocket(uint16_t port = 0) = 0;

        /**
         * Return a human readable from of a socket address.
         *
         * @param hAddr the handle of the socket address
         * @return a string representation of the socket address
         * @throws std::logic_error if the socket handle is invalid
         */
        virtual std::string GetAddressString(int hAddr) const = 0;
    };

} // end of namespace