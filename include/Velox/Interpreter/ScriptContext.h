/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes.
#include <memory>
#include <string>
#include <deque>
#include <vector>
#include <map>

namespace velox {

    // Forward declaration.
    class Item;
    class Scope;
    class ObjectType;

    /**
     * Represents the execution state of a script.
     * A script context contains local and global scopes as well as type definitions.
     */
    class ScriptContext {

    public:
        /** Constant for the flag which denotes that a return statement has been executed. */
        static const unsigned int RETURN_EXECUTED_FLAG;

        /** Constant for the flag which denotes that a break statement has been executed. */
        static const unsigned int BREAK_EXECUTED_FLAG;

        /** Constant for the flag which denotes that a continue statement has been executed. */
        static const unsigned int CONTINUE_EXECUTED_FLAG;


        /**
         * Searches for an object type.
         *
         * @param name  the name of the object type
         * @return the requested object type or 'nullptr' if no object type with the specified name could be found
         */
        std::shared_ptr<ObjectType> FindObjectType(const std::string &name);

        /**
         * Adds a new object type to this script context.
         *
         * @param name  the name of the object type
         * @param type  the object type
         * @throws std::logic_error in case the name if ambiguous
         */
        void AddObjectType(const std::string &name, std::shared_ptr<ObjectType> type);

        /**
         * Tests whether an object type with a specific name exists.
         *
         * @param name  the name of the object type
         * @return `true` if the object types exits
         */
        [[nodiscard]]
        bool HasObjectType(const std::string &name) const;

        /**
         * Sets flags according to the specified bitmask.
         *
         * @param bitmask   the bitmask for the flags to set
         */
        void SetFlag(unsigned int bitmask);

        /**
         * Clears the flags according to the specified bitmask.
         *
         * @param bitmask   the bitmask for the flags to clear
         */
        void ClearFlag(unsigned int bitmask);

        /**
         * Return the flag bitmask.
         *
         * @return the flags
         */
        unsigned int GetFlags() const;

        /**
         * Tests whether flags according to the specified bitmasks are set.
         *
         * @param bitmask   the bitmask to test
         * @return `true` if all of the flags according to the bitmask are set
         */
        bool IsFlagSet(unsigned int bitmask);

        /**
         * Clears all previously set flags.
         */
        void ClearFlags();

        void PushInstant(std::shared_ptr<Item> instant);

        bool HasInstant() const;

        void PopInstant();

        std::shared_ptr<Item> PeekInstant();

        /**
         * Adds a new return value on the return stack.
         *
         * @param value the return value to push or `nullptr` if an undefined item type should be pushed
         */
        void PushReturnValue(std::shared_ptr<Item> value = nullptr);

        /**
         * Removes and returns that latest added return value from the return stack.
         * In case the return value stack is empty, the behaviour is undefined.
         *
         * @return  the latest added return value
         */
        std::shared_ptr<Item> PopReturnValue();

        /**
         * Sets the value for the latest added return value.
         *
         * @param value the new return value
         */
        void SetCurrentReturnValue(std::shared_ptr<Item> value);

        /**
         * Clears the stack of return values.
         */
        void ClearReturnValues();

        /**
         * Tests whether an item exits either in the current local scope or within all global scopes.
         *
         * @param name  the name of the item
         * @return `true` if the item exits
         */
        bool HasItem(const std::string &name) const;

        /**
         * Searches for an item in the current local item as well as in all global scopes.
         *
         * @param name  the name of the requested item
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<Item> FindItem(const std::string &name);

        /**
         * Searches for an item in the current local item as well as in all global scopes.
         *
         * @param name  the name of the requested item
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<const Item> FindItem(const std::string &name) const;

        /**
         * Searches for an item in the current local item as well as in all global scopes.
         *
         * @param name          the name of the requested item
         * @param lineNumber    the line number within the source script.
         * @throws InterpreterError in case the item is unknown
         */
        Item &GetItem(const std::string &name, unsigned int lineNumber);

        /**
         * Searches for an item in the current local item as well as in all global scopes.
         *
         * @param name          the name of the requested item
         * @throws std::logic_error in case the item is unknown
         */
        Item &GetItem(const std::string &name);

        /**
         * Searches for an item in the current local item as well as in all global scopes.
         *
         * @param name          the name of the requested item
         * @throws std::logic_error in case the item is unknown
         */
        const Item &GetItem(const std::string &name) const;

        /**
         * Searches for an item in the current local item as well as in all global scopes.
         *
         * @param name          the name of the requested item
         * @param lineNumber    the line number within the source script.
         * @return the requested item
         * @throws InterpreterError in case the item is unknown
         */
        const Item &GetItem(const std::string &name, unsigned int lineNumber) const;

        /**
         * Adds a local item to the current local scope.
         * If the item name is ambiguous, the behaviour is undefined.
         *
         * @param name  the name of the item
         * @param item  the item to add
         */
        void AddLocalItem(const std::string &name, std::shared_ptr<Item> item);


        ///**
        // * Adds an item to this context.
        // *
        // * If no local scope exits, the item is added to the global scope.
        // * If the item name is ambiguous, the behaviour is undefined.
        // *
        // * @param name  the name of the item
        // * @param item  the item to add
        // */
        //void AddItem(const std::string& name, std::shared_ptr<Item> item);

        /**
         * Adds an anonymous item to the current local scope.
         *
         * @param item  the item to add
         */
        void AddItem(std::shared_ptr<Item> item);

        /**
         * Tests whether an item exits within the current local scope.
         *
         * @param name  the name of the item
         * @return `true` if the item exits
         */
        bool HasLocalItem(const std::string &name) const;

        /**
         * Adds an item to the hierarchy of global scopes.
         *
         * @param name  the name of the item
         * @param item  the item to add
         * @throws std::logic_error in case the name is not unique within the global scopes, or no global scope
         *                          are available
         */
        void AddGlobalItem(const std::string &name, std::shared_ptr<Item> item);

        /**
         * Tests whether an item within the hierarchy of global scopes exits.
         *
         * @param name  the name of the item
         * @return `true` if the item exits
         */
        bool HasGlobalItem(const std::string &name) const;

        /**
         * Searches for an local item within the current local scope.
         *
         * @param name  the name of the requested item
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<Item> FindLocalItem(const std::string &name);

        /**
         * Searches for an local item within the current local scope.
         *
         * @param name  the name of the requested item
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<const Item> FindLocalItem(const std::string &name) const;

        /**
         * Searches for an global item within all global scopes.
         *
         * @param name  the name of the requested item
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<Item> FindGlobalItem(const std::string &name);

        /**
         * Searches for an global item within all global scopes.
         *
         * @param name  the name of the requested item
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<const Item> FindGlobalItem(const std::string &name) const;

        /**
         * Adds a new scope on the stack of local scopes.
         */
        void PushGlobalScope(std::shared_ptr<Scope> scope = nullptr);

        /**
         * Removes the last added scope on the stack of global scopes.
         */
        void PopGlobalScope();

        /**
         * Removes all global scopes.
         */
        void ClearGlobalScopes();

        /**
         * Adds a new function scope on the stack of local scopes.
         * Code function scopes do not allow searching upwards in the hierarchy of local scopes.
         *
         * @param scope the scope to add or `nullptr` if a new empty scope should be added.
         */
        void PushFunctionScope(std::shared_ptr<Scope> scope = nullptr);

        /**
         * Adds a new code block scope on the stack of local scopes.
         * Code block scopes allow searching upwards in the hierarchy of local scopes.
         *
         * @param scope the scope to add or `nullptr` if a new empty scope should be added.
         */
        void PushCodeBlockScope(std::shared_ptr<Scope> scope = nullptr);

        /**
         * Removes the last added scope on the stack of global scopes.
         */
        void PopLocalScope();

        /**
         * Removes all local scopes.
         */
        void ClearLocalScopes();

        /**
         * Tests whether at least one local scope exits.
         *
         * @return `true` if a local scope exits
         */
        bool HasLocalScope() const
        {
            return !localScopes.empty();
        }

        /**
         * Returns the number of global scopes.
         *
         * @return the number of global scopes
         */
        size_t NumGlobalScopes() const;

        /**
         * Returns the number of local scopes.
         *
         * @return the number of local scopes
         */
        size_t NumberOfLocalScopes() const;

    private:
        /** The hierarchy of global scopes. */
        std::vector<std::shared_ptr<Scope>> globalScopes;

        /** The hierarchy of local scopes. */
        std::vector<std::shared_ptr<Scope>> localScopes;

        /** Stack of return values, used by functions and return statements. */
        std::vector<std::shared_ptr<Item>> returnValueStack;

        /** Stack of instant items values. */
        std::vector<std::shared_ptr<Item>> instantStack;

        /** Type definitions for custom objects. */
        std::map<std::string, std::shared_ptr<ObjectType>> objectTypes;

        /** Flags to be set and queried during execution of scripts. */
        unsigned int flags;
    };

}


