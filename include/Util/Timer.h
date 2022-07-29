/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <chrono>

namespace astu {

    /**
     * A timer class to be used as stop watch measuring time intervals.
     *
	 * @ingroup misc_group
     */
    class Timer {
    public:

        /**
         * Constructor.
         */
        Timer();

        /**
         * Starts the timer.
         *
         * If this timer is already running, calling this method has no effect.
         */
        void Start();

        /**
         * Stops the timer.
         *
         * If this timer is already stopped, calling this method has no effect.
         */
        void Stop();

        void Resume();

        /**
         * Returns whether this timer is currently running.
         *
         * @return `true` if this timer has been started
         */
        bool IsRunning() const {
            return running;
        }

        /**
         * Returns the elapsed time in milliseconds.
         *
         * @return the elapsed time in milliseconds (1/1000 seconds)
         */
        unsigned int GetMilliseconds() const;

        /**
         * Returns the elapsed time in microseconds.
         *
         * @return the elapsed time in microseconds (1/1000000 seconds)
         */
        unsigned int GetMicroseconds() const;


    private:
        /** Indicates whether this timer is currently running. */
        bool running;

        /** The point in time this timer has been started. */
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

        /** The elapsed time since this timer has been started. */
        std::chrono::nanoseconds elapsed;
    };

} // end of namespace


