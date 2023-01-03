/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "IInetSocketAddress.h"
#include "Util/Buffer.h"

// C++ Standard Library includes
#include <cassert>
#include <string>
#include <vector>

namespace astu {

    /**
     * Abstract base class for sockets.
     */
    class Socket {
    public:

        /** Virtual destructor. */
        virtual ~Socket() {}

        /**
         * Checks the status of this socket.
         * CallFunction this method at least once each main loop cycle.
         */
        virtual void Poll() = 0;

        /**
         * Checks whether messages have arrived on this socket.
         * CallFunction `Poll` on each main loop cycle before using this method.
         *
         * @return `true` if this socket has messages to read
         */
        virtual bool IsReadyToReceive() const = 0;

        /**
         * Checks whether messages can be sent via this socket.
         * CallFunction `Poll` on each main loop cycle before using this method.
         *
         * @return `true` if this socket can write messages
         */
        virtual bool IsReadyToSend() const = 0;

        /**
         * Sends a message through this socket.
         *
         * @param data      a vector containing the message content
         * @param hDestAddr the handle to the destination address
         * @throws std::runtime_error in case of an error
         */
        void SendTo(std::vector<unsigned char> data, int hDestAddr) {
            SendTo(data.data(), data.size(), hDestAddr);
        }

        /**
         * Sends a message through this socket.
         *
         * @param buffer    the buffer containing the message content
         * @param hDestAddr the handle to the destination address
         * @throws std::runtime_error in case of an error
         */
        void SendTo(const astu::Buffer & buffer, int hDestAddr) {
            SendTo(buffer.GetData(), buffer.GetRemaining(), hDestAddr);
        }

        /**
         * Sends a message through this socket.
         *
         * @param data      the message content
         * @param dataLng   the message length in bytes
         * @param hDestAddr handle to the target address
         * @throws std::runtime_error in case of an error
         */
        virtual void SendTo(const unsigned char *data, size_t dataLng, int hDestAddr) = 0;

        /**
         * Receives a message through this socket.
         *
         * @param buffer        the buffer where to write the message content.
         * @param bufferLng     the size of the buffer.
         * @param hDestAddr     receives the address handle of the sender
         * @return the actual number of bytes received
         * @throws std::runtime_error in case of an error
         */
        virtual size_t Receive(unsigned char *buffer, size_t bufferLng, int &hDestAddr) = 0;

        /**
         * Receives data from this socket.
         *
         * @param buffer receives the data
         * @return the address handle of the sender
         */
        int Receive(Buffer &buffer) {
            int hDestAddr;

            // We write directly into the buffer, which will not change its current position.
            size_t numBytes = Receive(buffer.GetCurrentData(), buffer.GetRemaining(), hDestAddr);

            // Now let's update the current position by skipping the number of bytes read.
            assert(buffer.GetRemaining() >= numBytes);
            buffer.SkipBytes(numBytes);

            return hDestAddr;
        }
    };

} // end of namespace
