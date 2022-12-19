// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Parser/FastSource.h"

// C++ Standard Library includes
#include <cctype>

namespace velox {

    TokenType FastSource::GetNextTokenType()
    {
        return TokenType::ADD;
    }

    void FastSource::EatWhitespace()
    {
        while (curChar != -1 && isspace(curChar)) {
            ReadChar();
        }
    }

    void FastSource::ReadChar()
    {
        //if (GetStream().IsGo)
    }


} // end of namespace