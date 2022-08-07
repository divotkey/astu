/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "InetSocketAddress.h"
#include "Socket.h"
#include "Network/IpMode.h"


// C++ Standard Library includes
#include <memory>

namespace astu {

    // Forward declaration
    class AddrInfo;

    /**
     * Operating system specific implementation of the network module.
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

        void SetIpMode(IpMode mode);
        IpMode GetIpMode() const;
        std::unique_ptr<Socket> CreateUdpSocket(uint16_t port);
        std::unique_ptr<Socket> CreateUdpSocket(const std::string &host, uint16_t port);
        std::unique_ptr<IInetSocketAddress> CreateUdpSocketAddress(const std::string & host, uint16_t port);

    private:
        IpMode ipMode;
        std::unique_ptr<Socket> CreateSocket(AddrInfo &addrInfo);

    };

} // end of namespace

