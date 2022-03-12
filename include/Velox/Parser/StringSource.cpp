// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "StringSource.h"

#define IS_END_OF_FILE() (pos >= source.size())

namespace velox {

    StringSource::StringSource(const std::string &source) : source(source), pos(0) {
        // Intentionally left empty.
    }

    char StringSource::GetFirstChar() {
        pos = 0;
        return GetCurrentChar();
    }

    char StringSource::GetNextChar() {
        ++pos;
        return GetCurrentChar();
    }

    char StringSource::GetCurrentChar() const {
        if (IS_END_OF_FILE()) {
            return 0;
        }

        return source[pos];
    }

    bool StringSource::IsEndOfFile() const {
        return IS_END_OF_FILE();
    }

    size_t StringSource::GetCharPosition() const {
        return pos;
    }

}