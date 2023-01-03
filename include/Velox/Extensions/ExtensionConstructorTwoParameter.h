/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
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
    class ExtensionConstructorTwoParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<T> (ScriptContext &sc,
                                                          Item &param1,
                                                          Item &param2,
                                                          unsigned int lineNumber)>;

        /**
         * Convenient method creating an a function item with an instance of this class.
         *
         * @param func  the one-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func) {
            return Item::CreateFunction(std::make_shared<ExtensionConstructorTwoParameter>(func));
        }

        /**
         * Constructor
         *
         * @param func  the actual function which processes one parameter
         */
        ExtensionConstructorTwoParameter(Func func) : func(func) {
            AddFormalParameter("a");
            AddFormalParameter("b");
        }

    protected:

        // Inherited via InterpreterFunction
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override {
            // Get the newly constructed item and add the required data.
            auto newItem = sc.FindItem("this");
            assert(newItem);

            auto newData = std::make_shared<T>();
            newItem->SetData(func(sc, sc.GetItem("a"), sc.GetItem("b"), lineNumber));

            return newItem;
        }

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}



