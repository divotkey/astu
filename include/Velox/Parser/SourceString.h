// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Source.h"

// C++ Standard Library includes
#include <sstream>
#include <string>

namespace velox {

    class SourceString : public Source {
    public:

        SourceString(const std::string& inSource);

    protected:
        std::shared_ptr<std::istream> GetStream() override;

    private:
        std::string source;
    };

}



