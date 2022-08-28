/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/Socket.h"
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
    class NetworkImpl;

    /**
     * Operating system specific implementation of Socket.
     */
    class SocketImpl : public Socket {
    public:

        /**
         * Constructor.
         *
         * @param network   the network implementation
         * @param hSocket   the socket handle
         * @throws std::logic_error in case the handle is invalid
         */
        SocketImpl(std::shared_ptr<NetworkImpl> network, int hSocket);

         /**
          * Constructor.
          *
          * @param network  the network implementation
          * @param domain   the socket domain (address family)
          * @param type     the communication semantics
          * @param protocol the communication protocol
          * @throws std::run_time error in case the socket could not be created
          */
        SocketImpl(std::shared_ptr<NetworkImpl> network, int domain, int type, int protocol = 0);

        /**
         * Destructor.
         */
        ~SocketImpl() override;

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

        // Inherited via Socket
        void Poll() override;
        void SendTo(const unsigned char *buf, size_t lng, int hAddr) override;

        bool IsReadyToReceive() const override;
        size_t Receive(unsigned char *buffer, size_t bufferLng, int &hDestAddr) override;
        //int Receive(Buffer &buffer) override;


    private:
        /** The implementation of the network core system. */
        std::shared_ptr<NetworkImpl> network;

        /** The socket handle. */
        int hSocket;

        /** Poll file descriptor used to poll the status of this socket. */
        struct pollfd pfd;

        /**
         * Called by all constructors.
         **/
        void InitSocket();

        /**
         * Initializes the data structure describing polling request.
         */
        void InitPoolFds();

        /**
         * Sets the socket into non-blocking mode.
         */
        void SetToNonBlocking();
    };

} // namespace astu