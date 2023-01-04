/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "SocketImpl.h"
#include "InetSocketAddress.h"
#include "AddrInfo.h"
#include "NetworkImpl.h"

// Linux API includes
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <iostream>

using namespace std;

namespace astu {

    SocketImpl::SocketImpl(std::shared_ptr<NetworkImpl> network, int _hSocket)
        : network(network), hSocket(_hSocket)
    {
        if (hSocket < 0) {
            throw logic_error("Invalid socket handle");
        }

        InitSocket();
    }

    astu::SocketImpl::SocketImpl(std::shared_ptr<NetworkImpl> network, int domain, int type, int protocol)
        : network(network), hSocket(-1)
    {
        // Create socket
        hSocket = socket(domain, type, protocol);
        if (hSocket < 0) {
            throw runtime_error(
                    string("Unable to create UDP socket: ")
                    + strerror(errno));
        }

        InitSocket();
    }

    SocketImpl::~SocketImpl()
    {
        if (hSocket >= 0) {
            close(hSocket);
        }
    }

    void SocketImpl::Bind(const struct sockaddr *addr, socklen_t len)
    {
        assert(hSocket >= 0);
        if (bind(hSocket, addr, len) < 0) {
            throw std::runtime_error(
                    "Unable to bind socket ("
                    + to_string(errno) + "): "
                    + strerror(errno));
        }
    }

    void SocketImpl::Poll()
    {
        auto result = poll(&pfd, 1, 0);
        if (result < 0) {
            throw std::runtime_error("Error polling socket ("
                + to_string(errno) + "): " + strerror(errno));
        }
    }

    bool SocketImpl::IsReadyToReceive() const
    {
        return pfd.revents & POLLIN;
    }

    bool SocketImpl::IsReadyToSend() const
    {
        return pfd.revents & POLLOUT;
    }

    void SocketImpl::InitPoolFds()
    {
        assert(hSocket >= 0);
        pfd.fd = hSocket;
        pfd.events = POLLIN | POLLOUT;
        pfd.revents = 0;
    }

    void SocketImpl::SendTo(const unsigned char *buf, size_t lng, int hAddr)
    {
        auto &addr = network->GetAddress(hAddr);
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
        } else if (cnt < lng) {
            // TODO log warning message
            cerr << "not all bytes could be sent to " << network->GetAddress(hAddr) << endl;
        }
    }

    //int SocketImpl::Receive(Buffer &buffer)
    //{
    //    struct sockaddr addr;
    //    socklen_t addrLen;
    //
    //    buffer.Clear();
    //    auto ret = recvfrom(hSocket,
    //             buffer.GetData(),
    //             buffer.GetCapacity(),
    //             MSG_DONTWAIT,
    //             &addr,
    //             &addrLen);
    //
    //    if (ret < 0) {
    //        throw std::logic_error("recvfrom returned " + to_string(ret) + ", error: " + strerror(errno) );
    //    }
    //
    //    buffer.SetLimit(ret);
    //
    //    return network->GetOrCreateAddressHandle(UniversalInetSocketAddress(&addr));
    //}

    size_t SocketImpl::Receive(unsigned char *buffer, size_t bufferLng, int &hDestAddr)
    {
        sockaddr addr;
        socklen_t addrLen = sizeof(sockaddr);

        auto ret = recvfrom(hSocket,
                            buffer,
                            bufferLng,
                            MSG_DONTWAIT,
                            &addr,
                            &addrLen);

        if (ret < 0) {
            throw std::logic_error(
                    "recvfrom returned " + to_string(ret) + ", error (" + to_string(errno) + "): " + strerror(errno));
        }

        hDestAddr = network->GetOrCreateAddressHandle(UniversalInetSocketAddress(&addr));
        return ret;
    }


    void SocketImpl::SetToNonBlocking()
    {
        int flags = fcntl(hSocket, F_GETFL);
        if (flags == -1) {
            throw runtime_error(string("Unable to get socket flags: ") + strerror(errno));
        }

        if (fcntl(hSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
            throw runtime_error(string("Unable to set socket flags: ") + strerror(errno));
        }
    }

    void SocketImpl::InitSocket()
    {
        InitPoolFds();
        SetToNonBlocking();
    }


} // end of namespace
