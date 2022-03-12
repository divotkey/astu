// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Parser/SyntaxNode.h"

namespace velox {

    class InterpreterNode : SyntaxNode {
    public:

    private:
    };

    class InterpreterStatementNode : InterpreterNode {
    public:
        virtual void Interpret() const;
    };

}



