/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Parser/FastSource.h"
#include "Util/Memento.h"

namespace astu {

    class SourceUtils {
    public:

        /**
         * Deleted constructor -> no instances of this class allowed.
         */
        SourceUtils() = delete;
        SourceUtils(const SourceUtils&) = delete;
        SourceUtils(SourceUtils&&) = delete;

        /**
         * Outputs the tokenized source as binary data.
         *
         * @param source    the source to store
         */
        static void PrintBinaryMemento(velox::FastSource &source);

    };

} // end of namespace