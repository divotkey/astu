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

    using ProgressSignalService = SignalServiceTs<ProgressSignal>;

} // end of namespace
