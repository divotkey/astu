/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

namespace astu {

    /**
     * Priorities define the sequence services get started, updated, etc.
     * 
     * @ingroup srv_group
     */
    enum Priority {
        /** Services with very high priority will be started and updated first. */
        VeryHigh = 0,

        /** These services will be started and updated after services with very high priority. */
        High = 500,

        /** These services will be started and updated after services with high priority. */
        Normal = 1000,

        /** These services will be started and updated after services with normal priority. */
        Low = 1500,

        /** Services with very low priority will be started and updated last. */
        VeryLow = 2000
    };

} // end of namespace