/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "UdpSocket.h"

// Linux API includes
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// C++ Standard Library includes
#include <stdexcept>

namespace astu {

    astu::UdpSocket::UdpSocket(int domain, int type, int protocol)
        : hSocket(-1)
    {
        // Create socket
        hSocket = socket(domain, type, protocol);
        if (hSocket < 0) {
            throw std::runtime_error(
                    std::string("Unable to create UDP socket: ")
                    + strerror(errno));
        }
    }

    UdpSocket::~UdpSocket()
    {
        if (hSocket >= 0) {
            close(hSocket);
        }
    }

    bool UdpSocket::Bind(struct sockaddr *addr, socklen_t len)
    {
        if (bind(hSocket, addr, len) < 0) {
            errorNumber = errno;
            errorMessage = strerror(errno);
            return false;
        }

        return true;
    }

} // end of namespace
