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
    class ExtensionFunctionNoParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<Item> (ScriptContext &sc,
                                                          T& exItem,
                                                          unsigned int lineNumber)>;

        /**
         * Convenient method creating a item of type function.
         *
         * @param func  the one-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func) {
            return Item::CreateFunction(make_shared<ExtensionFunctionNoParameter>(func));
        }

        /**
         * Constructor
         * @param func  the actual function which requires one parameter
         */
        ExtensionFunctionNoParameter(Func func) : func(func) {}

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override {
            auto exItem = std::dynamic_pointer_cast<T>(sc.FindItem("this")->GetData());
            assert(exItem);
            return func(sc, *exItem, lineNumber);
        }

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}



