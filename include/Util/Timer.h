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
     * A hTimer class to be used as stop watch measuring time intervals.
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
         * Starts the hTimer.
         *
         * If this hTimer is already running, calling this method has no effect.
         */
        void Start();

        /**
         * Stops the hTimer.
         *
         * If this hTimer is already stopped, calling this method has no effect.
         */
        void Stop();

        void Resume();

        /**
         * Returns whether this hTimer is currently running.
         *
         * @return `true` if this hTimer has been started
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
        /** Indicates whether this hTimer is currently running. */
        bool running;

        /** The point in time this hTimer has been started. */
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

        /** The elapsed time since this hTimer has been started. */
        std::chrono::nanoseconds elapsed;
    };

} // end of namespace


