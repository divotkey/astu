// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "InterpreterFunction.h"

// C++ Standard Library includes
#include <functional>

namespace velox {

    class InterpreterFunctionNoParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<Item>(unsigned int lineNumber)>;

        /**
         * Convenient method creating a item of type function.
         *
         * @param func  the two-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func);

        /**
         * Constructor
         * @param func  the actual function which requires one parameter
         */
        InterpreterFunctionNoParameter(Func func);

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override;

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}


