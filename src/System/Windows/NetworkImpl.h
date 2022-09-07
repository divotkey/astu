/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/IpMode.h"
#include "Network/Socket.h"

// C++ Standard Library includes
#include <memory>
#include <map>

namespace astu {

    /**
     * Operating system specific implementation of the network module.
     * This implementation class is used via the Pimpl-idiom by the network module.
     */
    class NetworkImpl : public std::enable_shared_from_this<NetworkImpl> {
    public:

        /**
         * Constructor.
         */
        NetworkImpl();

        /**
         * Destructor.
         */
        ~NetworkImpl();

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
        int GetAddressHandle(const std::string& host, uint16_t port) const;

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

        std::unique_ptr<Socket> CreateUdpSocket(uint16_t port);
        std::unique_ptr<Socket> CreateUdpSocket(const std::string &host, uint16_t port);
        std::string GetAddressString(int hAddr) const;

    private:
        /** Defines whether to use IPv4 or IPv6 or both address spaces. */
        IpMode ipMode;

        /** Used to generated unique address handles. */
        int cntHandles;
    };

} // end of namespace

