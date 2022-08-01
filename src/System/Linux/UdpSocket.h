/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/IUdpSocket.h"

// Linux API includes
#include <sys/socket.h>

// C++ Standard Library includes
#include <string>

namespace astu {

    /**
     * Operating system specific implementation of IUdpSocket.
     */
    class UdpSocket : public IUdpSocket {
    public:

         /**
          * Constructor.
          *
          * @param domain
          * @param type
          * @param protocol
          */
        UdpSocket(int domain, int type, int protocol = 0);

        /**
         * Destructor.
         */
        virtual ~UdpSocket();

        /**
         * Binds this socket to the specified address.
         *
         * @param addr  the address used to bind this socket
         * @param len   the length of the socket address
         */
        bool Bind(struct sockaddr *addr, socklen_t len);

        /**
         * Return the handle of this socket.
         *
         * @return the socket handle
         */
        int GetHandle() const {
            return hSocket;
        }

        /**
         * Returns the error number in case something went wrong.
         *
         * @return  the error number
         */
        int GetError() const {
            return errorNumber;
        }

        /**
         * Returns the error message in case something went wrong.
         *
         * @return  the error message
         */
        const std::string& GetErrorMessage() const {
            return errorMessage;
        }

        // Inherited via IUdpSocket

    private:
        /** The socket handle. */
        int hSocket;

        /** The error message, in case something went wrong. */
        std::string errorMessage;

        /** The error number in case something went wrong. */
        int errorNumber;
    };

} // namespace astu