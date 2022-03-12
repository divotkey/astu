// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "SyntaxNode.h"
#include "Source.h"

// C++ Standard Library includes
#include <memory>

namespace velox {

    class Parser {
    public:

        std::unique_ptr<SyntaxNode> Parse(Source &source);

    private:
        std::unique_ptr<SyntaxNode> ParseStatementSequence(Source &source);
        std::unique_ptr<SyntaxNode> ParseStatement(Source &source);
    };

}



