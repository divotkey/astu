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
         * Adds a global function that is available to all executed scripts and functions.
         *
         * @param name      the name of the function
         * @param function  the function
         */
        void AddFunction(const std::string &name, std::shared_ptr<InterpreterFunction> function);

        /**
         * Add a global object instant type that is available to all executed scripts and functions.
         *
         * @param instantDef    the instant definition statement
         */
        void AddInstant(InterpreterInstantDefinition &instantDef);

        /**
         * Adds a global object type (class/instant) that is available to all executed scripts.
         *
         * @param name      the name of the object type
         * @param objType   the object type
         */
        void AddObjectType(const std::string &name, std::shared_ptr<ObjectType> objType);

        /**
         * Adds a global floating-point constant that is available to all executed scripts.
         *
         * @param name  the name of the constant
         * @param value the value of the constant
         */
        void AddRealConstant(const std::string & name, double value);

        /**
         * Adds a global integer constant that is available to all executed scripts.
         *
         * @param name  the name of the constant
         * @param value the value of the constant
         */
        void AddIntConstant(const std::string & name, int value);

        /**
         * Adds a global item that is available to all executed scripts.
         *
         * @param name  the name of the item
         * @param item  the item to add
         */
        void AddGlobal(const std::string &name, std::shared_ptr<Item> item);

        /**
         * Queries whether an object type has already been added.
         *
         * @param name  the name of the object type
         * @return `true` if the object type has already been added
         */
        bool HasObjectType(const std::string &name) const;

        /**
         * Executes the specified program
         * @param program
         */
        void Execute(std::shared_ptr<InterpreterStatement> program);

        /**
         * Calls a function that takes zero parameters.
         *
         * @param item  the function item.
         */
        void CallWithNoParams(Item& item);

        /**
         * Calls a function with one single integer value as parameters.
         *
         * @param item  the function item.
         */
        void CallWithIntParam(Item& item, int value);

        /**
         * Calls a function with one single item as parameters.
         *
         * @param item  the function item.
         */
        void CallWithOneParam(Item& item, std::shared_ptr<Item> param);

        /**
         * Calls a function with a list of parameters.
         *
         * @param item      the function item.
         * @param params    the list of parameters
         */
        void Call(Item& item, InterpreterActualParameterList &params);

    private:
        /** The script context of this interpreter, representing the script's execution state. */
        ScriptContext context;
    };

}
