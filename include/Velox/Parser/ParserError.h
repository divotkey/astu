// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// C++ Standard Library includes
#include <stdexcept>

namespace velox {

    class ParserError : public std::runtime_error {
    public:

        ParserError(const std::string& message, unsigned int line = 0)
            : std::runtime_error(message), lineNumber(line) {
            // Intentionally left empty.
        }

        unsigned int GetLineNumber() const {
            return lineNumber;
        }

        virtual ~ParserError() {}

    private:
        unsigned int lineNumber;
    };

}
