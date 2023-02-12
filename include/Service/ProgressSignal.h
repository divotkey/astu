/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "SignalServiceTs.h"

namespace astu {

    class ProgressSignal final {

    public:
        /** Describes what percentage of the current task has been completed. */
        double completed;

        ProgressSignal(double completed) : completed(completed) {}

    };

    /**
     * Type definition for signal listeners which receive progress signals.
     *
     * @ingroup srv_group
     */
    using IProgressListener = ISignalListener<ProgressSignal>;

    /**
     * Type definition for signal services used to transmit progress signals.
     *
     * @ingroup srv_group
     */
    using ProgressSignalService = SignalServiceTs<ProgressSignal>;

    /**
     * Services can derive from this class to process progress signals.
     *
     * @ingroup srv_group
     */
    class ProgressSignalListener
            : virtual public Service
            , private IProgressListener
    {
    public:

        /**
         * Constructor.
         */
        ProgressSignalListener() {
            AddStartupHook([this](){
                ASTU_SERVICE(ProgressSignalService).AddListener(*this);
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(ProgressSignalService).RemoveListener(*this);
            });
        }

    protected:

        /**
         * Called by this base class when a progress update has been received.
         *
         * @param completed  percentage of completion of the current task
         */
        virtual bool OnProgressUpdate(double completed) {
            return false;
        }

    private:

        // Inherited via MouseButtonListener
        virtual bool OnSignal(const ProgressSignal &signal) {
            return OnProgressUpdate(signal.completed);
        }

    };

} // end of namespace
