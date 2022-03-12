// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "TokenType.h"

// C++ Standard Library includes
#include <string>

namespace velox {

    class Token {
    public:

        Token(TokenType type, size_t pos) : type(type), pos(pos) {
            // Intentionally left empty.
        }

        void Reset() {
            type = TokenType::INVALID;
            sValue.clear();
            pos = 0;
        }

        bool IsValid() const;

        /**
         * Returns a string representation of the type of this token.
         *
         * @return the token type as string.
         */
        const std::string GetTypeAsString() const;

        /** The type of this token. */
        TokenType type;

        /** The linear position within the source code, starting from zero. */
        size_t pos;

        /** The string of this token as it appeared in the source code. */
        std::string sValue;
    };

}



