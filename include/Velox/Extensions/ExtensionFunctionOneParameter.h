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
    class ExtensionFunctionOneParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<Item> (ScriptContext &sc,
                                                          T& exItem,
                                                          Item &param,
                                                          unsigned int lineNumber)>;

        /**
         * Convenient method creating an a function item with an instance of this class.
         *
         * @param func  the one-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func) {
            return Item::CreateFunction(std::make_shared<ExtensionFunctionOneParameter>(func));
        }

        /**
         * Constructor
         *
         * @param func  the actual function which processes one parameter
         */
        ExtensionFunctionOneParameter(Func func) : func(func) {
            AddFormalParameter("a");
        }

    protected:

        // Inherited via InterpreterFunction
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override {
            auto exItem = std::dynamic_pointer_cast<T>(sc.FindItem("this")->GetData());
            assert(exItem);
            return func(sc, *exItem, sc.GetItem("a", lineNumber), lineNumber);
        }

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}



