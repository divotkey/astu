/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "AddrInfo.h"

// Linux API includes
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// C++ Standard Library includes
#include <stdexcept>

using namespace std;

namespace astu {

    AddrInfo::AddrInfo()
        : ipMode(IpMode::IPv4), result (nullptr), current(nullptr)
    {
        // Intentionally left empty;
    }

    AddrInfo::~AddrInfo()
    {
        Cleanup();
    }

    void AddrInfo::RetrieveUdpAddresses(const std::string &host, uint16_t port)
    {
        Cleanup();

        struct addrinfo hints = {};
        SetFamily(hints);
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = IPPROTO_UDP;

        int err = getaddrinfo(
                host.c_str(),
                to_string(port).c_str(),
                &hints,
                &result
                );

        if (err != 0) {
            throw runtime_error(
                    string("unable to retrieve host address: ")
                    + gai_strerror(err));
        }

        current = result;
    }

    void AddrInfo::RetrieveUdpAddresses(uint16_t port)
    {
        Cleanup();

        struct addrinfo hints = {};
        SetFamily(hints);
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;
        hints.ai_protocol = IPPROTO_UDP;

        int err = getaddrinfo(
                nullptr,
                to_string(port).c_str(),
                &hints,
                &result
        );

        if (err != 0) {
            throw runtime_error(
                    string("unable to retrieve host address: ")
                    + gai_strerror(err));
        }

        current = result;
    }

    bool AddrInfo::HasAddress() const
    {
        return current != nullptr;
    }

    void AddrInfo::NextAddress()
    {
        if (!current) {
            throw std::logic_error(
                    "Unable to go to next address, no more addresses found");
        }
        current = current->ai_next;
    }

    AddrInfo &AddrInfo::SetIpMode(IpMode mode)
    {
        ipMode = mode;
        return *this;
    }

    void AddrInfo::SetFamily(addrinfo &info)
    {
        switch (ipMode) {
            case IpMode::IPv4:
                info.ai_family = AF_INET;
                break;

            case IpMode::IPv6:
                info.ai_family = AF_INET6;
                break;

            case IpMode::IPv4_And_IPv6:
                info.ai_family = AF_UNSPEC;
                break;
        }
    }

    size_t AddrInfo::NumAddresses() const
    {
        auto addr = result;
        size_t cnt = 0;

        while (addr) {
            addr = addr->ai_next;
            ++cnt;
        }
        return cnt;
    }

    void AddrInfo::Cleanup()
    {
        if (result) {
            freeaddrinfo(result);
            result = nullptr;
        }
    }

    int AddrInfo::GetDomain() const
    {
        VerifyAddressInfo();
        return current->ai_family;
    }

    int AddrInfo::GetType() const
    {
        VerifyAddressInfo();
        return current->ai_socktype;
    }

    int AddrInfo::GetProtocol() const
    {
        VerifyAddressInfo();
        return current->ai_protocol;
    }

    const struct sockaddr *AddrInfo::GetAddr() const
    {
        VerifyAddressInfo();
        return current->ai_addr;
    }

    socklen_t AddrInfo::GetAddrLen() const
    {
        VerifyAddressInfo();
        return current->ai_addrlen;
    }

    void AddrInfo::VerifyAddressInfo() const
    {
        if (!current) {
            throw std::logic_error("End of address info list reached");
        }
    }

    std::ostream &operator<<(ostream &os, const AddrInfo &addrInfo)
    {
        const size_t cMaxLen = 256;
        char buf[cMaxLen];
        const char *result;
        uint16_t port;

        auto sockAddr = addrInfo.current->ai_addr;
        switch (sockAddr->sa_family) {

            case AF_INET: {
                struct sockaddr_in *sockAddrIp4;
                sockAddrIp4 = reinterpret_cast<struct sockaddr_in*>(sockAddr);
                result = inet_ntop(
                        AF_INET,
                        &sockAddrIp4->sin_addr,
                        buf,
                        cMaxLen);

                port = ntohs(sockAddrIp4->sin_port);
            }
                break;

            case AF_INET6: {
                struct sockaddr_in6 *sockAddrIp6;
                sockAddrIp6 = reinterpret_cast<struct sockaddr_in6*>(sockAddr);
                result = inet_ntop(
                        AF_INET6,
                        &sockAddrIp6->sin6_addr,
                        buf,
                        cMaxLen);

                port = ntohs(sockAddrIp6->sin6_port);
            }
                break;

            default:
                throw std::runtime_error("unknown address family");
        }

        if(!result) {
            throw runtime_error("unable to convert address to text: "
                                + to_string(errno));
        }

        if (sockAddr->sa_family == AF_INET6) {
            os << "[" << result << "]:" << port;
        } else {
            os << result << ":" << port;
        }

        return os;
    }

} // end of namespace

