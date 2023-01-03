/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterFunction.h"

// C++ Standard Library includes
#include <functional>

namespace velox {

    // Forward declaration
    class Item;

    class InterpreterFunctionThreeParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<Item>(ScriptContext & sc,
                                                         std::shared_ptr<Item> param1,
                                                         std::shared_ptr<Item> param2,
                                                         std::shared_ptr<Item> param3,
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
        InterpreterFunctionThreeParameter(Func func);

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override;

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}



