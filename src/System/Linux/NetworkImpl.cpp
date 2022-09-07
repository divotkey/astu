/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "NetworkImpl.h"
#include "AddrInfo.h"
#include "SocketImpl.h"
#include "InetSocketAddress.h"

// C++ Standard Library includes
#include <iostream>
#include <cassert>

using namespace std;

namespace astu {

    NetworkImpl::NetworkImpl()
        : ipMode(IpMode::IPv4_And_IPv6)
    {
        // Intentionally left empty
    }

    NetworkImpl::~NetworkImpl()
    {
        // Intentionally left empty
    }

    std::unique_ptr<SocketImpl> NetworkImpl::CreateUdpSocket(uint16_t port)
    {
        // Query UDP addresses.
        AddrInfo addrInfo;
        addrInfo.SetIpMode(ipMode);
        addrInfo.RetrieveUdpAddresses(port);

        return CreateSocket(addrInfo);
    }

    std::unique_ptr<SocketImpl> NetworkImpl::CreateUdpSocket(const string &host, uint16_t port)
    {
        AddrInfo addrInfo;
        addrInfo.SetIpMode(ipMode);
        addrInfo.RetrieveUdpAddresses(host, port);

        return CreateSocket(addrInfo);
    }

    std::unique_ptr<SocketImpl> NetworkImpl::CreateSocket(AddrInfo &addrInfo)
    {
        unique_ptr<SocketImpl> socket;
        while (addrInfo.HasAddress() && !socket) {
            // TODO log debug message about attempt to create UDP socket

            try {
                socket = make_unique<SocketImpl>(
                        shared_from_this(),
                        addrInfo.GetDomain(),
                        addrInfo.GetType(),
                        addrInfo.GetProtocol());

                socket->Bind(addrInfo.GetAddr(), addrInfo.GetAddrLen());
            } catch (const std::runtime_error & e) {
                // TODO log info/warning message about failed socket creation
                cerr << "unable to create socket for address " << addrInfo << endl;
                socket = nullptr;
            }

            if (!socket)
                addrInfo.NextAddress();
        }

        if (!socket) {
            throw std::runtime_error("Unable to create socket");
        }

        return socket;
    }

    //std::unique_ptr<IInetSocketAddress> NetworkImpl::CreateUdpSocketAddress(const string &host, uint16_t port)
    //{
    //    AddrInfo addrInfo;
    //    addrInfo.SetIpMode(ipMode);
    //    addrInfo.RetrieveUdpAddresses(host, port);
    //
    //    if (!addrInfo.HasAddress()) {
    //        throw std::runtime_error(
    //                "Unable to resolve UDP socket address for host '"
    //                + host + "' at port " + to_string(port));
    //    }
    //
    //    assert(addrInfo.GetType() == SOCK_DGRAM);
    //    assert(addrInfo.GetProtocol() == IPPROTO_UDP);
    //
    //    return make_unique<UniversalInetSocketAddress>(addrInfo.GetAddr());
    //
    //    //switch (addrInfo.GetDomain()) {
    //    //    case AF_INET:
    //    //        // IPv4 address.
    //    //
    //    //        return make_unique<InetSocketAddressIpv4>(
    //    //                reinterpret_cast<const struct sockaddr_in *>(addrInfo.GetAddr()));
    //    //
    //    //    case AF_INET6:
    //    //        // IPv6 address.
    //    //        return make_unique<InetSocketAddressIpv6>(
    //    //                reinterpret_cast<const struct sockaddr_in6 *>(addrInfo.GetAddr()));
    //    //
    //    //    default:
    //    //        throw std::runtime_error("Unknown address family");
    //    //}
    //}

    void NetworkImpl::SetIpMode(IpMode mode)
    {
        ipMode = mode;
    }

    IpMode NetworkImpl::GetIpMode() const
    {
        return ipMode;
    }

    int NetworkImpl::CreateAddressHandle(const string &host, uint16_t port)
    {
        return CreateAddressHandle(CreateAddress(host, port));
    }

    bool NetworkImpl::HasAddressHandle(const string &host, uint16_t port) const
    {
        return HasAddressHandle(CreateAddress(host, port));
    }

    int NetworkImpl::GetAddressHandle(const string &host, uint16_t port) const
    {
        return GetAddressHandle(CreateAddress(host, port));
    }

    bool NetworkImpl::HasAddressHandle(const UniversalInetSocketAddress &addr) const
    {
        return addressToHandle.find(addr) != addressToHandle.end();
    }

    int NetworkImpl::CreateAddressHandle(const UniversalInetSocketAddress &addr)
    {
        if (HasAddressHandle(addr)) {
            throw std::logic_error("Handle for address "
                + addr.GetAddressString() + " already created. Use HasAddressHandle() and GetAddressHandle().");
        }

        const int handle = ++cntHandles;
        addressToHandle.insert({addr, handle});
        handleToAddress.insert({handle, addr});

        return handle;
    }

    int NetworkImpl::GetAddressHandle(const UniversalInetSocketAddress &addr) const
    {
        for (auto it : handleToAddress) {
            if (it.second == addr)
                return it.first;
        }

        throw std::logic_error("Unable to retrieve address handle, unknown address. Use HasAddressHandle() and CreateAddressHandle()."
                               + addr.GetAddressString());

        //auto it = addressToHandle.find(addr);
        //if (it == addressToHandle.end()) {
        //    throw std::logic_error("Unable to retrieve address handle, unknown address. Use HasAddressHandle() and CreateAddressHandle()."
        //        + addr.GetAddressString());
        //}
        //
        //return it->second;
    }

    const UniversalInetSocketAddress &NetworkImpl::GetAddress(int hAddr) const
    {
        auto it = handleToAddress.find(hAddr);
        if (it == handleToAddress.end()) {
            throw std::logic_error("Unknown address handle " + to_string(hAddr));
        }

        return it->second;
    }

    UniversalInetSocketAddress NetworkImpl::CreateAddress(const string &host, uint16_t port) const
    {
        AddrInfo addrInfo;
        addrInfo.SetIpMode(ipMode);
        addrInfo.RetrieveUdpAddresses(host, port);

        if (!addrInfo.HasAddress()) {
            throw std::runtime_error(
                    "Unable to resolve UDP socket address for host '"
                    + host + "' at port " + to_string(port));
        }

        assert(addrInfo.GetType() == SOCK_DGRAM);
        assert(addrInfo.GetProtocol() == IPPROTO_UDP);
        return addrInfo.GetAddr();
    }

    std::string NetworkImpl::GetAddressString(int hAddr) const
    {
        auto & addr = GetAddress(hAddr);
        return addr.GetAddressString() + ":" + to_string(addr.GetPort());
    }

    int NetworkImpl::GetOrCreateAddressHandle(const string &host, uint16_t port)
    {
        return GetOrCreateAddressHandle( CreateAddress(host, port) );
    }

} // end of namespace
