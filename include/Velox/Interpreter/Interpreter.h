/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "InterpreterStatement.h"
#include "InterpreterActualParameterList.h"
#include "ScriptContext.h"
#include "Scope.h"

// C++ Standard Library includes
#include <stdexcept>
#include <memory>

namespace velox {

    // Forward declaration
    class InterpreterFunction;

    class InterpreterInstantDefinition;

    class ObjectType;

    /**
     * Interprets velox programs.
     */
    class Interpreter {
    public:

        /**
         * Constructor.
         *
         * Initializes a new interpreter, without any global functions, variables or constants
         */
        Interpreter();

        /**
         * Executes the specified statement.
         * Statements can also be statement sequences and hence represent an entire script.
         *
         * @param statement the statement to execute.
         */
        void Execute(InterpreterStatement &statement);

        /**
         * Calls a function that takes zero parameters.
         *
         * @param item  the function item
         * @throws InterpreterError in case the specified item is not of type function
         */
        void CallFunctionNoParam(Item &item);

        /**
         * Calls a function with one single integer value as parameters.
         *
         * @param item  the function item.
         * @param value the integer parameter
         * @throws InterpreterError in case the specified item is not of type function
         */
        void CallFunctionIntParam(Item &item, int value);

        /**
         * Calls a function with one single integer value as parameters.
         *
         * @param item  the function item.
         * @param value the floating-point parameter
         * @throws InterpreterError in case the specified item is not of type function
         */
        void CallFunctionRealParam(Item &item, double value);

        /**
         * Calls a function with one single item as parameters.
         *
         * @param item  the function item.
         * @throws InterpreterError in case the specified item is not of type function
         */
        void CallFunctionOneParam(Item &item, std::shared_ptr<Item> param);

        /**
         * Calls a function with a list of parameters.
         *
         * @param item      the function item
         * @param params    the list of parameters
         * @throws InterpreterError in case the specified item is not of type function
         */
        void CallFunction(Item &item, InterpreterActualParameterList &params);

        /**
         * Adds standard functions, variables and constants.
         */
        void AddStandardGlobals();

        /**
         * Adds another scope to the global scope stack.
         */
        void PushGlobalScope();

        /**
         * Removes the last pushed scope from the global scope stack.
         */
        void PopGlobalScope();

        /**
         * Returns the current number of global scopes.
         *
         * @return the number of global scopes
         */
        size_t NumGlobalScopes() const;

        /**
         * Adds a global function that is available to all executed scripts and functions.
         *
         * @param name      the name of the function
         * @param function  the function
         */
        void AddGlobalFunction(const std::string &name, std::shared_ptr<InterpreterFunction> function);

        /**
         * Add an instant type that is available to all executed scripts and functions.
         *
         * @param instantDef    the instant definition statement
         */
        void AddInstant(InterpreterInstantDefinition &instantDef);

        /**
         * Adds an object type (e.g., a class or instant) that is available to all executed scripts.
         *
         * @param name      the name of the object type
         * @param objType   the object type
         */
        void AddObjectType(const std::string &name, std::shared_ptr<ObjectType> objType);

        /**
         * Queries whether an object type has already been added.
         *
         * @param name  the name of the object type
         * @return `true` if the object type has already been added
         */
        bool HasObjectType(const std::string &name) const;

        /**
         * Adds a global floating-point constant that is available to all executed scripts.
         *
         * @param name  the name of the constant
         * @param value the value of the constant
         */
        void AddGlobalReal(const std::string &name, double value);

        /**
         * Adds a global integer constant that is available to all executed scripts.
         *
         * @param name  the name of the constant
         * @param value the value of the constant
         */
        void AddGlobalInteger(const std::string &name, int value);

        /**
         * Adds a global item that is available to all executed scripts.
         *
         * @param name  the name of the item
         * @param item  the item to add
         */
        void AddGlobal(const std::string &name, std::shared_ptr<Item> item);

    private:
        /** The script context of this interpreter, representing the script's execution state. */
        ScriptContext context;
    };

}
