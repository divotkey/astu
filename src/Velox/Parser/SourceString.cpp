// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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