/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */
#pragma once

// Linux API includes
#include <netdb.h>

// C++ Standard Library includes
#include <string>
#include <iostream>

namespace astu {

    /**
     * Utility class used to retrieve address information.
     */
    class AddrInfo {
    public:

        /**
         * Describes the allowed IP address versions.
         */
        enum class IpMode {IPv4, IPv6, IPv4_And_IPv6};

        /** Constructor. */
        AddrInfo();

        /** Destructor. */
        ~AddrInfo();

        /**
         * Specifies the IP mode.
         *
         * @param mode the IP mode
         * @return reference to this class for method chaining
         */
        AddrInfo& SetIpMode(IpMode mode);

        /**
         * Queries addresses of a host with the intention to use UDP.
         *
         * The intended use of this method is to get the address of a
         * remote server, with the intention to send UDP messages.
         *
         * @param host  the server name
         * @param port  the server port
         * @throws std::runtime_error in case of an error
         */
        void GetUdpAddresses(const std::string &host, uint16_t port);

        /**
         * Queries addresses of this machine with the intention to use UDP.
         *
         * The intended use of this method is to get the local address of
         * a machine to bind an UDP socket used to receive UDP messages.
         *
         * @param port the UDP port to be used
         * @throws std::runtime_error in case of an error
         */
        void GetUdpAddresses(uint16_t port);

        /**
         * Return the number of retrieved addresses.
         *
         * @return the number of addresses
         */
        size_t NumAddresses() const;

        /**
         * Returns whether there is a retrieved address.
         *
         * @return `true` if there is a retrieved address
         */
        bool HasAddress() const;

        /**
         * Moves on to the next retrieved address.
         *
         * @throws std::logic_error in case there is currently no address
         */
        void NextAddress();

    private:
        /** Describes the allowed IP address mode. */
        IpMode ipMode;

        /** The retrieved address info structure. */
        struct addrinfo *result;

        /** Pointer to the currently selected address. */
        struct addrinfo *current;

        void SetFamily(struct addrinfo& info);
        void Cleanup();

        friend std::ostream& operator<<(std::ostream&, const AddrInfo &);
    };

    std::ostream& operator<<(std::ostream& os, const AddrInfo &addrInfo);

} // end of namespace

