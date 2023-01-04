/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "InetSocketAddress.h"
#include "SocketImpl.h"
#include "Network/IpMode.h"

// C++ Standard Library includes
#include <memory>
#include <map>

namespace astu {

    // Forward declaration
    class AddrInfo;

    /**
     * Operating system specific implementation of the network module.
     * This implementation class is used via the Pimpl-idiom by the network module.
     */
    class NetworkImpl : public std::enable_shared_from_this<NetworkImpl>{
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

        /**
         * Returns the socket address associated by the specified handle.
         *
         * @param hAddr the handle of the socket address
         * @return the associated socket address
         * @throws std::logic_error in case the specified handle is invalid
         */
        const UniversalInetSocketAddress& GetAddress(int hAddr) const;

        /**
         * Tests whether a address handle is registered for the specified socket address.
         *
         * @param addr  the socket address
         * @return `true` if a address handle is known for the socket address
         */
        bool HasAddressHandle(const UniversalInetSocketAddress &addr) const;

        /**
         * Creates a new socket address handle for the specified socket address.
         *
         * @param addr  the socket address
         * @return the newly created handle
         * @throws std::logic_error in case the socket address is already associated with an address handle
         */
        int CreateAddressHandle(const UniversalInetSocketAddress & addr);

        /**
         * Returns the address handle for the specified socket address.
         *
         * @param addr  the socket address
         * @return the address handle associated with the socket address
         * @throws std::logic_error in case the socket address is unknown
         */
        int GetAddressHandle(const UniversalInetSocketAddress & addr) const;

        /**
         * Convenient method used to get a address handle for the specified socket address.
         * In case the specified socket address has not yet been associated with an address handle,
         * a new address handle is created.
         *
         * @param addr the socket address
         * @return the  address handle for the specified socket address
         */
        int GetOrCreateAddressHandle(const UniversalInetSocketAddress & addr) {
            if (HasAddressHandle(addr)) {
                return GetAddressHandle(addr);
            }
            return CreateAddressHandle(addr);
        }

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
        std::unique_ptr<SocketImpl> CreateUdpSocket(uint16_t port);

        /**
         * Return a human readable from of a socket address.
         *
         * @param hAddr the handle of the socket address
         * @return a string representation of the socket address
         * @throws std::logic_error if the socket handle is invalid
         */
        std::string GetAddressString(int hAddr) const;

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
        std::unique_ptr<SocketImpl> CreateUdpSocket(const std::string &host, uint16_t port);

    private:
        /** Defines whether to use IPv4 or IPv6 or both address spaces. */
        IpMode ipMode;

        /** Used to generated unique address handles. */
        int cntHandles;

        /** Associates handles with socket addresses. */
        std::map<int, UniversalInetSocketAddress> handleToAddress;

        /** Associates socket addresses  with handles. */
        std::map<UniversalInetSocketAddress, int> addressToHandle;


        std::unique_ptr<SocketImpl> CreateSocket(AddrInfo &addrInfo);
        UniversalInetSocketAddress CreateAddress(const std::string& host, uint16_t port) const;

        int GetOrCreateHandle(const UniversalInetSocketAddress & addr) {
            if (HasAddressHandle(addr)) {
                return CreateAddressHandle(addr);
            } else {
                return GetAddressHandle(addr);
            }
        }
    };

} // end of namespace

