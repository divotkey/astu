/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stdexcept>

namespace astu {

    class ScannerError : public std::runtime_error {
    public:

        ScannerError(const std::string& message, unsigned int line = 0)
            : std::runtime_error(message), lineNumber(line) {
            // Intentionally left empty.
        }

        unsigned int GetLineNumber() const {
            return lineNumber;
        }

        virtual ~ScannerError() {}

    private:
        unsigned int lineNumber;

    };

}
