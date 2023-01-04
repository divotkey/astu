/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ItemState.h"
#include "Velox/Interpreter/InterpreterFunction.h"

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
        CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, std::shared_ptr<Scope> memberScope, unsigned int lineNumber) override;

    private:
        /** The interpreter function to be executed. */
        std::shared_ptr<InterpreterFunction> function;
    };

}



