// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "ScriptContext.h"

namespace velox {

    class InterpreterStatement {
    public:

        InterpreterStatement(unsigned int lineNumber = 0) : lineNumber (lineNumber) {};

        virtual ~InterpreterStatement() {}
        virtual void Execute(ScriptContext &sc) = 0;
        virtual void Prepare(ScriptContext &sc) {}

        unsigned int GetLineNumber() const {
            return lineNumber;
        }

    private:
        /** Information about the location within the source code */
        unsigned int lineNumber;
    };

}


