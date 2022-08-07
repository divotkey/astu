/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/ISocket.h"
#include "InetSocketAddress.h"

// Linux API includes
#include <sys/socket.h>
#include <sys/poll.h>

// C++ Standard Library includes
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace astu {

    // Forward declaration
    class InetSocketAddress;
    class NetworkImpl;

    /**
     * Operating system specific implementation of ISocket.
     */
    class Socket : public ISocket {
    public:

        /**
         * Creates a new socket.
         *
         * @param network   the network implementation
         * @param domain    the socket domain (address family)
         * @param type      the communication semantics
         * @param protocol  the communication protocol
         * @return the newly created socket or `nullptr` in case the socket
         *         could not be created
         */
        static std::unique_ptr<Socket> Create(std::shared_ptr<NetworkImpl>, int domain, int type, int protocol = 0);

        /**
         * Creates a new socket and binds it to the specified socket address.
         *
         * @param network   the network implementation
         * @param domain    the socket domain (address family)
         * @param type      the communication semantics
         * @param protocol  the communication protocol
         * @param addr      the address used to bind this socket
         * @param len       the length of the socket address
         * @return
         */
        static std::unique_ptr<Socket> Create(
                std::shared_ptr<NetworkImpl>,
                int domain,
                int type,
                int protocol,
                const struct sockaddr *addr,
                socklen_t len);

        /**
         * Constructor.
         *
         * @param network   the network implementation
         * @param hSocket   the socket handle
         * @throws std::logic_error in case the handle is invalid
         */
        explicit Socket(std::shared_ptr<NetworkImpl> network, int hSocket);

         /**
          * Constructor.
          *
          * @param network  the network implementation
          * @param domain   the socket domain (address family)
          * @param type     the communication semantics
          * @param protocol the communication protocol
          * @throws std::run_time error in case the socket could not be created
          */
        Socket(std::shared_ptr<NetworkImpl> network, int domain, int type, int protocol = 0);

        /**
         * Destructor.
         */
        ~Socket() override;

        /**
         * Binds this socket to the specified address.
         *
         * @param addr  the address used to bind this socket
         * @param len   the length of the socket address
         * @throws std::runtime_error
         */
        void Bind(const struct sockaddr *addr, socklen_t len);

        /**
         * Return the handle of this socket.
         *
         * @return the socket handle
         */
        int GetHandle() const {
            return hSocket;
        }

        // Inherited via ISocket
        void Poll() override;
        void SendTo(unsigned char *buf, size_t lng, const IInetSocketAddress &addr) override;
        void SendTo(unsigned char *buf, size_t lng, int hAddr) override;

        bool IsReadyToRead() const override;
        void Read();

        int CreateAddressHandle(const std::string & host, uint16_t port) override;
        bool HasAddressHandle(int hAddr) const override;
        const IInetSocketAddress& GetAddressFromHandle(int hAddr) const override;

    private:
        /** The implementation of the network core system. */
        std::shared_ptr<NetworkImpl> network;

        /** The socket handle. */
        int hSocket;

        /** Counts the number of generated address handles. */
        int cntHandles;

        /** Poll file descriptor used to poll the status of this socket. */
        struct pollfd pfd;

        /** Associates integers (handles) with socket addresses. */
        std::map<int, UniversalInetSocketAddress> handleToAddress;

        /** Associates socket addresses  with integers (handles). */
        std::map<UniversalInetSocketAddress, int> addressToHandle;

        void InitPoolFds();
        const UniversalInetSocketAddress& GetAddress(int hAddr) const;
    };

} // namespace astu