/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

namespace velox {

    /**
     * Base class for all custom data that might get attached to items.
     */
    class ItemData {
    public:

        /** Virtual destructor. */
        virtual ~ItemData() {}
    };

} // end of namespace