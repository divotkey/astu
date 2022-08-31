// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterFunction.h"

// C++ Standard Library includes
#include <functional>

namespace velox {

    // Forward declaration
    class Item;

    class InterpreterFunctionTwoParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<Item>(ScriptContext &sc,
                                                         std::shared_ptr<Item> param1,
                                                         std::shared_ptr<Item> param2,
                                                         unsigned int lineNumber)>;

        /**
         * Convenient method creating a item of type function.
         *
         * @param func  the two-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func);

        /**
         * Constructor
         *
         * @param func  the actual function which requires one parameter
         */
        InterpreterFunctionTwoParameter(Func func);

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override;

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}



