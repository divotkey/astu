// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "ItemState.h"
#include "InterpreterFunction.h"

namespace velox {

    class ItemStateFunction : public ItemState {
    public:

        /**
         * Constructor.
         *
         * @param function  the interpreter function to be executed
         */
        ItemStateFunction(std::shared_ptr<InterpreterFunction> function) : function(function) {};

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        ItemType GetType() const override;

        std::shared_ptr<Item>
        CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, unsigned int lineNumber) override;

    private:
        /** The interpreter function to be executed. */
        std::shared_ptr<InterpreterFunction> function;
    };

}



