/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "NetworkImpl.h"
#include "AddrInfo.h"
#include "UdpSocket.h"

// C++ Standard Library includes
#include <iostream>

using namespace std;

namespace astu {

    NetworkImpl::NetworkImpl()
    {
        // Intentionally left empty
    }

    NetworkImpl::~NetworkImpl()
    {
        // Intentionally left empty
    }

    std::unique_ptr<UdpSocket> NetworkImpl::CreateUdpSocket(uint16_t port)
    {
        AddrInfo addrInfo;
        addrInfo.SetIpMode(AddrInfo::IpMode::IPv4);
        addrInfo.GetUdpAddresses(port);

        cout << "found " << addrInfo.NumAddresses() << " address(es)" << endl;

        while (addrInfo.HasAddress()) {
            cout << addrInfo << endl;
            addrInfo.NextAddress();
        }

        return nullptr;
    }

} // end of namespace
