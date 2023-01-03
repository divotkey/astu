/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "NetworkImpl.h"

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
        return -1;
    }

    bool NetworkImpl::HasAddressHandle(const string &host, uint16_t port) const
    {
        return false;
    }

    int NetworkImpl::GetAddressHandle(const string &host, uint16_t port) const
    {
        return -1;
    }

    int NetworkImpl::GetOrCreateAddressHandle(const string &host, uint16_t port)
    {
        return -1;
    }

    std::unique_ptr<Socket> NetworkImpl::CreateUdpSocket(uint16_t port)
    {
        return nullptr;
    }

    std::unique_ptr<Socket> NetworkImpl::CreateUdpSocket(const string &host, uint16_t port)
    {
        return nullptr;
    }

    std::string NetworkImpl::GetAddressString(int hAddr) const {
        return "";
    }

} // end of namespace
