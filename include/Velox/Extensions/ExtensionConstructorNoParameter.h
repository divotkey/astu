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
    class ExtensionConstructorNoParameter : public velox::InterpreterFunction {
    public:

        using Func = std::function<std::shared_ptr<T> (ScriptContext &sc, unsigned int lineNumber)>;

        /**
         * Convenient method creating an a function item with an instance of this class.
         *
         * @param func  the no-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func) {
            return Item::CreateFunction(std::make_shared<ExtensionConstructorNoParameter>(func));
        }

        /**
         * Constructor
         */
        ExtensionConstructorNoParameter(Func func) : func(func) {
            // Intentionally left empty.
        }

    protected:

        // Inherited via InterpreterFunction
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override {
            // Get the newly constructed item and add the required data.
            auto newItem = sc.FindItem("this");
            assert(newItem);

            newItem->SetData(func(sc, lineNumber));

            return newItem;
        }

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

}



