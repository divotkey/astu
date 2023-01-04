/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/InterpreterFunction.h"

// C++ Standard Library includes
#include <functional>
#include <cassert>

namespace velox {

    template<typename T>
    class ExtensionFunctionTwoParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<Item> (ScriptContext &sc,
                                                          T& exItem,
                                                          Item &param1,
                                                          Item &param2,
                                                          unsigned int lineNumber)>;

        /**
         * Convenient method creating an a function item with an instance of this class.
         *
         * @param func  the two-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func) {
            return Item::CreateFunction(std::make_shared<ExtensionFunctionTwoParameter>(func));
        }

        /**
         * Constructor
         *
         * @param func  the actual function which processes two parameters
         */
        ExtensionFunctionTwoParameter(Func func) : func(func) {
            AddFormalParameter("a");
            AddFormalParameter("b");
        }

    protected:

        // Inherited via InterpreterFunction
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override {
            auto exItem = std::dynamic_pointer_cast<T>(sc.FindItem("this")->GetData());
            assert(exItem);
            return func(sc, *exItem, sc.GetItem("a"), sc.GetItem("b"), lineNumber);
        }

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

} // end of line



