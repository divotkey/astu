/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include <stdexcept>

namespace velox {

    class InterpreterError : public std::runtime_error {
    public:

        InterpreterError(const std::string& message, unsigned int line = 0)
                : std::runtime_error(message), lineNumber(line) {
            // Intentionally left empty.
        }

        virtual ~InterpreterError() {}

        unsigned int GetLineNumber() const {
            return lineNumber;
        }

    private:
        unsigned int lineNumber;

    };

}

