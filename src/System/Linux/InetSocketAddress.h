/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/IInetSocketAddress.h"

// Linux API includes
#include <netinet/in.h>

namespace astu {

    /**
     * Wrapper class for socket addresses.
     *
     * This class is capable of handling both, IPv4 and IPv6 addresses and
     * avoids using polymorphism and hence eliminates the need to create
     * instanced on the heap.
     */
    class UniversalInetSocketAddress : public IInetSocketAddress {
    public:

        /**
         * Casts an IInetSocketAddress& to an UniversalInetSocketAddress&.
         *
         * @param addr  the interface
         * @return the casted InetSocketAddress
         * @throws std::logic_error in case the dynamic type does not match
         */
        static const UniversalInetSocketAddress & Cast(const IInetSocketAddress &addr);

        /**
         * Constructor.
         *
         * @param inAddr
         */
        UniversalInetSocketAddress(const struct sockaddr *inAddr);

        /**
         * Returns whether this socket address is an IPv4 address.
         *
         * @return `true` if this address is an IPv4 address
         */
        bool IsIPv4() const;

        /**
         * Returns whether this socket address is an IPv6 address.
         *
         * @return `true` if this address is an IPv6 address
         */
        bool IsIPv6() const;

        /**
         * Return the raw pointer to the address.
         *
         * @return the address
         */
        const struct sockaddr *GetAddress() const;

        /**
         * Returns the size of the address structure.
         *
         * @return the address structure size
         */
        size_t GetAddressLength() const;

        /**
         * Requires for use as key in maps etc.
         *
         * @param rhs the right hand side for this operator
         * @return 'true' if this address is considers "less" than the rhs
         */
        bool operator< (const UniversalInetSocketAddress& rhs) const;

        // Inherited via IInetSocketAddress
        const std::string& GetAddressString() const final;
        uint16_t GetPort() const override;
        void PrintToStream(std::ostream &os) const override;
        bool operator< (const IInetSocketAddress& rhs) const override;

    private:
        /** Stores both Ipv4 and Ipv6 socket addresses. */
        struct sockaddr_storage addr;

        /** The string representation of this address, lazy initialized. */
        mutable std::string addressString;

        void FetchAddressString() const;
        static bool IsLess(const struct sockaddr_in &lhs, const struct sockaddr_in &rhs);
        static bool IsLess(const struct sockaddr_in6 &lhs, const struct sockaddr_in6 &rhs);
    };

    class InetSocketAddress : public IInetSocketAddress {
    public:

        /**
         * Casts an IInetSocketAddress to an InetSocketAddress.
         *
         * @param addr  the interface
         * @return the casted InetSocketAddress
         * @throws std::logic_error in case the dynamic type does not match
         */
        static const InetSocketAddress & Cast(const IInetSocketAddress &addr);

        virtual const struct sockaddr *GetAddr() const = 0;
        virtual size_t GetAddrLength() const = 0;

        // Inherited via IInetSocketAddress
        const std::string& GetAddressString() const final;

    protected:
        /** The string representation of the address. */
        mutable std::string addressString;

        virtual void FetchAddressString() const = 0;
    };

    class InetSocketAddressIpv4 : public InetSocketAddress {
    public:

        /**
         * Constructor.
         *
         * @param addr  the ipv4 socket address data structure
         */
        InetSocketAddressIpv4(const struct sockaddr_in *addr);

        // Inherited via InetSocketAddress
        const struct sockaddr *GetAddr() const override;
        size_t GetAddrLength() const override;

        // Inherited via IInetSocketAddress
        uint16_t GetPort() const override;
        void PrintToStream(std::ostream &os) const override;

    protected:
        void FetchAddressString() const override;

    private:
        /** The actual address structure. */
        struct sockaddr_in addr;
    };

    class InetSocketAddressIpv6 : public InetSocketAddress {
    public:

        /**
         * Constructor.
         *
         * @param addr  the ipv6 socket address data structure
         */
        InetSocketAddressIpv6(const struct sockaddr_in6 *addr);

        // Inherited via InetSocketAddress
        const struct sockaddr *GetAddr() const override;
        size_t GetAddrLength() const override;

        // Inherited via IInetSocketAddress
        uint16_t GetPort() const override;
        void PrintToStream(std::ostream &os) const override;

    protected:
        void FetchAddressString() const override;

    private:
        /** The actual address structure. */
        struct sockaddr_in6 addr;

    };

} // end of namespace

