// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "InterpreterExpression.h"

namespace velox {

    class InterpreterLiteralList : public InterpreterExpression {
    public:

        /**
         * Constructor.
         */
        InterpreterLiteralList() : InterpreterExpression(false) {}

        void AddElement(std::shared_ptr<InterpreterExpression> elem);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The values of this literal. */
        std::vector<std::shared_ptr<InterpreterExpression>> elements;
    };

}


