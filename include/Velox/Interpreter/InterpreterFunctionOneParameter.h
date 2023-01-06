/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterFunction.h"

// C++ Standard Library includes
#include <functional>

namespace velox {

    // Forward declaration
    class Item;

    /**
     * Utility class to use delegates for interpreter functions.
     *
     * Note: the parameter item is deliberately passed as shared pointer instead of a reference. This
     * is required for interpreter-internal usage of this class, e.g. in ItemStateList.
     */
    class InterpreterFunctionOneParameter : public velox::InterpreterFunction {
    public:

        /**
         * Alias for the delegate to be called
         */
        using Func = std::function<std::shared_ptr<Item> (ScriptContext &sc,
                                                          std::shared_ptr<Item> param,
                                                          unsigned int lineNumber)>;

        /**
         * Convenient method creating a item of type function.
         *
         * @param func  the one-parameter function
         * @return the newly created item
         */
        static std::shared_ptr<Item> CreateItem(Func func);

        /**
         * Constructor
         * @param func  the actual function which requires one parameter
         */
        InterpreterFunctionOneParameter(Func func);

    protected:
        std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) final override;

    private:
        /** The actual function this wrapper will use. */
        Func func;
    };

} // end of namespace



