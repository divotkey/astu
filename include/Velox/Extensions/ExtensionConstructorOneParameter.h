// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/InterpreterFunction.h"

// C++ Standard Library includes
#include <functional>
#include <cassert>

namespace velox {

    template<typename T>
    class ExtensionConstructorOneParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<T> (ScriptContext &sc,
                                                          Item &param,
                                                          unsigned int lineNumber)>;

        /**
         * Convenient method creating an a function item with an instance of this class.
         *
         * @param func  the one-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func) {
            return Item::CreateFunction(make_shared<ExtensionConstructorOneParameter>(func));
        }

        /**
         * Constructor
         *
         * @param func  the actual function which processes one parameter
         */
        ExtensionConstructorOneParameter(Func func) : func(func) {
            AddFormalParameter("a");
        }

    protected:

        // Inherited via InterpreterFunction
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override {
            // Get the newly constructed item and add the required data.
            auto newItem = sc.FindItem("this");
            assert(newItem);

            auto newData = std::make_shared<T>();
            newItem->SetData(func(sc, sc.GetItem("a"), lineNumber));

            return newItem;
        }

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}



