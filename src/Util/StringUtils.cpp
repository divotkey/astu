/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Util/StringUtils.h"

using namespace std;

namespace astu {

    string astu::StringUtils::ExtractFileExtension(const string &filename, bool keepPeriod) {
        auto idx = filename.find_last_of('.');
        if (idx == string::npos) {
            return "";
        }

        return filename.substr(idx + (keepPeriod ? 0 : 1));
    }

    string StringUtils::StripFileExtension(const string &filename) {
        auto idx = filename.find_last_of('.');
        return filename.substr(0, idx);
    }

} // end of namespace

