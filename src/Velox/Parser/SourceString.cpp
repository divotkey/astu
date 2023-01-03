/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Parser/SourceString.h"

using namespace std;

namespace velox {

    SourceString::SourceString(const std::string &inSource) : source(inSource) {
        Reset();
    }

    std::shared_ptr<std::istream> SourceString::GetStream() {
        return make_shared<stringstream>(source);
    }

} // end of namespace