/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Socket.h"
#include "InetSocketAddress.h"
#include "AddrInfo.h"
#include "NetworkImpl.h"

// Linux API includes
#include <unistd.h>
#include <errno.h>
#include <string.h>

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <iostream>

using namespace std;

namespace astu {

    std::unique_ptr<Socket> Socket::Create(std::shared_ptr<NetworkImpl> network, int domain, int type, int protocol)
    {
        int hSocket = socket(domain, type, protocol);
        if (hSocket < 0)
            return nullptr;

        return make_unique<Socket>(network, hSocket);
    }

    std::unique_ptr<Socket>
    Socket::Create(std::shared_ptr<NetworkImpl> network, int domain, int type, int protocol, const struct sockaddr *addr, socklen_t len)
    {
        int hSocket = socket(domain, type, protocol);
        if (hSocket < 0) {
            return nullptr;
        }

        if (bind(hSocket, addr, len) < 0) {
            return nullptr;
        }

        return make_unique<Socket>(network, hSocket);
    }

    Socket::Socket(std::shared_ptr<NetworkImpl> network, int _hSocket)
        : network(network), hSocket(_hSocket), cntHandles(0)
    {
        if (hSocket < 0) {
            throw logic_error("Invalid socket handle");
        }
        InitPoolFds();
    }

    astu::Socket::Socket(std::shared_ptr<NetworkImpl> network, int domain, int type, int protocol)
        : network(network), hSocket(-1), cntHandles(0)
    {
        // Create socket
        hSocket = socket(domain, type, protocol);
        if (hSocket < 0) {
            throw runtime_error(
                    string("Unable to create UDP socket: ")
                    + strerror(errno));
        }

        InitPoolFds();
    }

    Socket::~Socket()
    {
        if (hSocket >= 0) {
            close(hSocket);
        }
    }

    void Socket::Bind(const struct sockaddr *addr, socklen_t len)
    {
        assert(hSocket >= 0);
        if (bind(hSocket, addr, len) < 0) {
            // TODO log error
            cerr << "unable to bind socket (" << errno << "): " << strerror(errno) << endl;
            throw std::runtime_error(
                    "Unable to bind socket ("
                    + to_string(errno) + "): "
                    + strerror(errno));
        }
    }

    //void Socket::Update()
    //{
    //    Poll();
    //
    //    dirty = true;
    //}

    void Socket::Poll()
    {
        auto result = poll(&pfd, 1, 0);
        if (result < 0) {
            throw std::runtime_error("Error polling socket ("
                + to_string(errno) + "): " + strerror(errno));
        }
    }

    bool Socket::IsReadyToRead() const
    {
        return pfd.revents | POLLOUT;
    }

    void Socket::InitPoolFds()
    {
        assert(hSocket >= 0);
        pfd.fd = hSocket;
        pfd.events = POLLIN & POLLOUT;
        pfd.revents = 0;
    }

    void Socket::SendTo(unsigned char *buf, size_t lng, const IInetSocketAddress &addr)
    {
        auto &addrImpl = UniversalInetSocketAddress::Cast(addr);
        auto cnt = sendto(
                hSocket,
                buf,
                lng,
                0,
                addrImpl.GetAddress(),
                addrImpl.GetAddressLength());

        if (cnt < 0) {
            throw std::runtime_error("Unable to send message to "
                + addr.GetAddressString() + ": " + strerror(errno) );
        } else if (cnt << lng) {
            // TODO log warning message
        }
    }

    void Socket::SendTo(unsigned char *buf, size_t lng, int hAddr)
    {
        auto &addr = GetAddress(hAddr);
        auto cnt = sendto(
                hSocket,
                buf,
                lng,
                0,
                addr.GetAddress(),
                addr.GetAddressLength());

        if (cnt < 0) {
            throw std::runtime_error("Unable to send message to "
                                     + addr.GetAddressString() + ": " + strerror(errno) );
        } else if (cnt << lng) {
            // TODO log warning message
        }
    }


    int Socket::CreateAddressHandle(const string &host, uint16_t port)
    {
        AddrInfo addrInfo;
        addrInfo.SetIpMode(network->GetIpMode());
        addrInfo.RetrieveUdpAddresses(host, port);

        if (!addrInfo.HasAddress()) {
            throw std::runtime_error(
                    "Unable to resolve UDP socket address for host '"
                    + host + "' at port " + to_string(port));
        }

        assert(addrInfo.GetType() == SOCK_DGRAM);
        assert(addrInfo.GetProtocol() == IPPROTO_UDP);

        assert(handleToAddress.size() == addressToHandle.size());

        UniversalInetSocketAddress addr(addrInfo.GetAddr());
        auto it = addressToHandle.find(addr);
        if (it == addressToHandle.end()) {
            addressToHandle[addr] = ++cntHandles;
            assert(!HasAddressHandle(cntHandles));
            handleToAddress.insert({cntHandles, addr});
            assert(handleToAddress.size() == addressToHandle.size());
            return cntHandles;
        }

        return it->second;
    }

    bool Socket::HasAddressHandle(int hAddr) const
    {
        return handleToAddress.find(hAddr) != handleToAddress.end();
    }

    const IInetSocketAddress &Socket::GetAddressFromHandle(int hAddr) const
    {
        auto it = handleToAddress.find(hAddr);
        if (it == handleToAddress.end()) {
            throw std::logic_error("unknown address handle " + to_string(hAddr));
        }

        return it->second;
    }

    const UniversalInetSocketAddress &Socket::GetAddress(int hAddr) const
    {
        assert(handleToAddress.size() == addressToHandle.size());

        auto it = handleToAddress.find(hAddr);
        if (it == handleToAddress.end()) {
            throw std::logic_error("unknown address handle " + to_string(hAddr));
        }

        return it->second;
    }

} // end of namespace
