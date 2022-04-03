// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// C++ Standard Library includes
#include <stdexcept>

namespace astu {

    class ScannerException : public std::runtime_error {
    public:

        ScannerException(const std::string& message) : std::runtime_error(message) {
            // Intentionally left empty.
        }

        virtual ~ScannerException() {}

    };

}
