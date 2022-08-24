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
#include <map>

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

        /**
         * Specifies the IP mode.
         *
         * @param mode the IP mode
         * @return reference to this class for method chaining
         */
        void SetIpMode(IpMode mode);

        /**
         * Returns the currently set IP mode.
         *
         * @return the current IP mode
         */
        IpMode GetIpMode() const;

        int CreateAddressHandle(const std::string& host, uint16_t port);
        const UniversalInetSocketAddress& GetAddress(int hAddr) const;

        std::unique_ptr<Socket> CreateUdpSocket(uint16_t port);
        std::unique_ptr<Socket> CreateUdpSocket(const std::string &host, uint16_t port);
        std::unique_ptr<IInetSocketAddress> CreateUdpSocketAddress(const std::string & host, uint16_t port);

    private:
        /** Defines whether to use IPv4 or IPv6 or both address spaces. */
        IpMode ipMode;

        /** Used to generated unique address handles. */
        int cntHandles;

        /** Associates handles with socket addresses. */
        std::map<int, UniversalInetSocketAddress> handleToAddress;

        /** Associates socket addresses  with handles. */
        std::map<UniversalInetSocketAddress, int> addressToHandle;


        std::unique_ptr<Socket> CreateSocket(AddrInfo &addrInfo);
        bool HasHandle(const UniversalInetSocketAddress &addr) const;
        int CreateHandle(const UniversalInetSocketAddress & addr);
        int GetHandle(const UniversalInetSocketAddress & addr) const;

        int GetOrCreateHandle(const UniversalInetSocketAddress & addr) {
            if (HasHandle(addr)) {
                return CreateHandle(addr);
            } else {
                return GetHandle(addr);
            }
        }
    };

} // end of namespace

