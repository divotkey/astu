/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "InetSocketAddress.h"

// Linux API includes
#include <arpa/inet.h>

// C++ Standard Library includes
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <cstring>

// Some macros to shorten code
#define ADDR4 reinterpret_cast<const struct sockaddr_in*>(&addr)
#define ADDR6 reinterpret_cast<const struct sockaddr_in6*>(&addr)
#define USE_ADDR4(a) reinterpret_cast<const struct sockaddr_in*>(&a)
#define USE_ADDR6(a) reinterpret_cast<const struct sockaddr_in6*>(&a)

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// class InetSocketAddress
    /////////////////////////////////////////////////

    const InetSocketAddress &InetSocketAddress::Cast(const IInetSocketAddress &addr)
    {
        auto result = dynamic_cast<const InetSocketAddress*>(&addr);
        if (!result) {
            throw std::logic_error("Invalid dynamic type of IInetSocketAddress");
        }
        return *result;
    }

    const std::string &InetSocketAddress::GetAddressString() const
    {
        if (addressString.empty()) {
            FetchAddressString();
        }

        return addressString;
    }

    /////////////////////////////////////////////////
    /////// class InetSocketAddressIpv4
    /////////////////////////////////////////////////

    InetSocketAddressIpv4::InetSocketAddressIpv4(const struct sockaddr_in *inAddr)
            : addr(*inAddr)
    {
        // Intentionally left empty.
    }

    uint16_t InetSocketAddressIpv4::GetPort() const
    {
        return ntohs(addr.sin_port);
    }

    void InetSocketAddressIpv4::FetchAddressString() const
    {
        const size_t cMaxLen = 16;
        char buf[cMaxLen];

        auto result = inet_ntop(
                AF_INET,
                &addr.sin_addr,
                buf,
                cMaxLen);

        if (!result) {
            throw runtime_error("Unable to convert address to text: "
                                + to_string(errno));
        }

        addressString = buf;
    }

    void InetSocketAddressIpv4::PrintToStream(ostream &os) const
    {
        os << InetSocketAddress::GetAddressString() << ":" << GetPort();
    }

    const struct sockaddr *InetSocketAddressIpv4::GetAddr() const
    {
        return reinterpret_cast<const struct sockaddr*>(&addr);
    }

    size_t InetSocketAddressIpv4::GetAddrLength() const
    {
        return sizeof(struct sockaddr_in);
    }

    /////////////////////////////////////////////////
    /////// class InetSocketAddressIpv6
    /////////////////////////////////////////////////

    InetSocketAddressIpv6::InetSocketAddressIpv6(const struct sockaddr_in6 *inAddr)
            : addr(*inAddr)
    {
        // Intentionally left empty.
    }

    uint16_t InetSocketAddressIpv6::GetPort() const
    {
        return ntohs(addr.sin6_port);
    }

    void InetSocketAddressIpv6::FetchAddressString() const
    {
        const size_t cMaxLen = 48;
        char buf[cMaxLen];

        auto result = inet_ntop(
                AF_INET,
                &addr.sin6_addr,
                buf,
                cMaxLen);

        if (!result) {
            throw runtime_error("Unable to convert address to text: "
                                + to_string(errno));
        }

        addressString = buf;
    }

    void InetSocketAddressIpv6::PrintToStream(ostream &os) const
    {
        os << "[" << InetSocketAddress::GetAddressString() << "]:" << GetPort();
    }

    const struct sockaddr *InetSocketAddressIpv6::GetAddr() const
    {
        return reinterpret_cast<const struct sockaddr*>(&addr);
    }

    size_t InetSocketAddressIpv6::GetAddrLength() const
    {
        return sizeof(struct sockaddr_in6);
    }

    /////////////////////////////////////////////////
    /////// UniversalInetSocketAddress
    /////////////////////////////////////////////////

    bool UniversalInetSocketAddress::IsIPv4() const
    {
        return addr.ss_family == AF_INET;
    }

    bool UniversalInetSocketAddress::IsIPv6() const
    {
        return addr.ss_family == AF_INET6;
    }

    uint16_t UniversalInetSocketAddress::GetPort() const
    {
        switch (addr.ss_family) {

            case AF_INET:
                return ntohs(USE_ADDR4(addr)->sin_port);

            case AF_INET6:
                return ntohs(USE_ADDR6(addr)->sin6_port);

            default:
                // This should never happen.
                assert(false);
                return 0;
        }
    }

    const std::string &UniversalInetSocketAddress::GetAddressString() const
    {
        if (addressString.empty()) {
            FetchAddressString();
        }

        return addressString;
    }

    void UniversalInetSocketAddress::FetchAddressString() const
    {
        // We need a buffer, because inet_ntop returns a zero-terminated string
        // and assigning a char buffer to a string does the conversion for us.

        const size_t cMaxLen = 48;
        char buf[cMaxLen];

        const char* result;
        switch (addr.ss_family) {
            case AF_INET:
                result = inet_ntop(
                        AF_INET,
                        &ADDR4->sin_addr,
                        buf,
                        cMaxLen);
                break;

            case AF_INET6:
                result = inet_ntop(
                        AF_INET6,
                        &ADDR6->sin6_addr,
                        buf,
                        cMaxLen);
                break;

            default:
                // This should never happen.
                assert(false);
                break;
        }

        if (!result) {
            throw runtime_error("Unable to convert address to text: "
                                + to_string(errno));
        }

        addressString = buf;
    }

    const UniversalInetSocketAddress &UniversalInetSocketAddress::Cast(const IInetSocketAddress &addr)
    {
        auto result = dynamic_cast<const UniversalInetSocketAddress*>(&addr);
        if (!result) {
            throw std::logic_error("Invalid dynamic type of IInetSocketAddress");
        }
        return *result;
    }

    void UniversalInetSocketAddress::PrintToStream(ostream &os) const
    {
        switch (addr.ss_family) {
            case AF_INET:
                os << GetAddressString() << ":" << GetPort();
                break;

            case AF_INET6:
                os << "[" << GetAddressString() << "]:" << GetPort();
                break;

            default:
                os << "[unknown address family]";
        }
    }

    const struct sockaddr *UniversalInetSocketAddress::GetAddress() const
    {
        return reinterpret_cast<const struct sockaddr*>(&addr);
    }

    size_t UniversalInetSocketAddress::GetAddressLength() const
    {
        switch (addr.ss_family) {
            case AF_INET:
                // The length field is not required to be present in all
                // implementations: USE_ADDR4(addr)->sin_len
                return sizeof(struct sockaddr_in);

            case AF_INET6:
                // The length field is not required to be present in all
                // implementations: USE_ADDR6(addr)->sin_len
                return sizeof(struct sockaddr_in6);

            default:
                assert(false);
                break;
        }

        return 0;
    }

    UniversalInetSocketAddress::UniversalInetSocketAddress(const struct sockaddr *inAddr)
    {
        switch (inAddr->sa_family) {
            case AF_INET:
                *reinterpret_cast<struct sockaddr_in*>(&addr)
                        = *reinterpret_cast<const struct sockaddr_in*>(inAddr);
                break;

            case AF_INET6:
                *reinterpret_cast<struct sockaddr_in6*>(&addr)
                        = *reinterpret_cast<const struct sockaddr_in6*>(inAddr);
                break;
        }
    }

    bool UniversalInetSocketAddress::operator<(const UniversalInetSocketAddress &rhs) const
    {
        switch (addr.ss_family) {
            case AF_INET:
                switch (rhs.addr.ss_family) {
                    case AF_INET:
                        // Both are IPv4 addresses, do v4 comparison
                        return IsLess(
                                reinterpret_cast<const struct sockaddr_in &>(addr),
                                reinterpret_cast<const struct sockaddr_in &>(rhs)
                                );

                    case AF_INET6:
                        // IPv6 is considered to be greater than v4 addresses.
                        return true;
                }
                // Something went wrong.
                break;

            case AF_INET6:
                switch (rhs.addr.ss_family) {
                    case AF_INET:
                        // IPv6 is considered to be greater than v4 addresses.
                        return false;

                    case AF_INET6:
                        // Both are IPv6 addresses, do v6 comparison
                        return IsLess(
                                reinterpret_cast<const struct sockaddr_in6 &>(addr),
                                reinterpret_cast<const struct sockaddr_in6 &>(rhs)
                        );
                }
                // Something went wrong.
                break;
        }

        throw std::runtime_error("Unsupported address family");
    }

    bool UniversalInetSocketAddress::IsLess(const sockaddr_in &lhs, const sockaddr_in &rhs)
    {
        // Addresses and ports are in network byte order, but it's about
        // the consistency of the operator (strict weak ordering), not a 'real'
        // comparison.

        if (lhs.sin_addr.s_addr < rhs.sin_addr.s_addr)
            return true;

        if (lhs.sin_addr.s_addr > rhs.sin_addr.s_addr)
            return false;

        return lhs.sin_port < rhs.sin_port;
    }

    bool UniversalInetSocketAddress::IsLess(const sockaddr_in6 &lhs, const sockaddr_in6 &rhs)
    {
        // Addresses and ports are in network byte order, but it's about
        // the consistency of the operator (strict weak ordering), not a 'real'
        // comparison.

        auto cmp = memcmp(&lhs, &rhs, sizeof(sockaddr_in6));
        if (cmp < 0)
            return true;
        if (cmp > 0)
            return false;

        return lhs.sin6_port < rhs.sin6_port;
    }

    bool UniversalInetSocketAddress::operator<(const IInetSocketAddress &rhs) const
    {
        return *this < UniversalInetSocketAddress::Cast(rhs);
    }

    bool UniversalInetSocketAddress::operator==(const UniversalInetSocketAddress &rhs) const
    {
        size_t n1 = addr.ss_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
        size_t n2 = rhs.addr.ss_family == AF_INET ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in6);

        size_t n = min(n1, n2);

        return memcmp(&addr, &rhs.addr, min(n1, n2)) == 0;
    }

} // end of namespace
