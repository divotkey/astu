// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Source.h"

// C++ Standard Library includes
#include <string>

namespace velox {

    class StringSource : public Source {
    public:

        StringSource(const std::string& source);


        // Inherited via Source
        char GetCurrentChar() const override;
        bool IsEndOfFile() const override;
        size_t GetCharPosition() const override;

    protected:
        // Inherited via Source
        char GetFirstChar() override;
        char GetNextChar() override;

    private:
        /** The string containing the source code. */
        std::string source;

        /** The current position within the source code string. */
        size_t pos;
    };

}



