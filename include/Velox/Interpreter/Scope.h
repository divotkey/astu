/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes.
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace velox {

    // Forward declaration.
    class Item;
    class ObjectType;

    class Scope {
    public:

        /**
         * Constructor.
         */
        Scope()  = default;

        /**
         * Add anonymous item to current scope.
         *
         * @param item  the item to add
         */
        void AddItem(std::shared_ptr<Item> item);

        /**
         * Adds an item to this scope.
         *
         * In case the item name is ambiguous, the behaviour is undefined.
         *
         * @param name  the name of the item
         * @param item  the item
         */
        void AddItem(const std::string& name, std::shared_ptr<Item> item);

        /**
         * Tests whether a item exists in this scope.
         *
         * @param name  the name of the item to test
         * @return `true` if the item exists
         */
        bool HasItem(const std::string& name) const;

        /**
         * Searches for an element with the specified name in this scope and upwards in the hierarchy of scopes.
         *
         * @param name  the name of the item to search
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<Item> FindItem(const std::string& name);

        /**
         * Searches for an element with the specified name in this scope and upwards in the hierarchy of scopes.
         *
         * @param name  the name of the item to search
         * @return the requested item or `nullptr'
         */
        std::shared_ptr<const Item> FindItem(const std::string& name) const;

        /**
         * Searches for an object type in this scope and upwards in the hierarchy of scopes.
         *
         * @param name  the name of the object type
         * @return the requested object type or 'nullptr' if no object type with the specified name could be found
         */
        std::shared_ptr<ObjectType> FindObjectType(const std::string &name);

        /**
         * Tests whether an object type with a specific name exists.
         *
         * @param name              the name of the object type
         * @param searchHierarchy   whether to search upwards the hierarchy of scopes
         * @return `true` if the object types exits
         */
        [[nodiscard]]
        bool HasObjectType(const std::string &name, bool searchHierarchy) const;

        /**
         * Adds a new object type to this script context.
         *
         * @param name  the name of the object type
         * @param type  the object type
         * @throws std::logic_error in case the name if ambiguous
         */
        void AddObjectType(const std::string &name, std::shared_ptr<ObjectType> type);

        /**
         * Removes all items and object types in this scope.
         */
        void Clear();

    private:
        /** List of items associated with unique names. */
        std::map<std::string, std::shared_ptr<Item>> items;

        /** Holds items without names. */
        std::vector<std::shared_ptr<Item>> anonymousItems;

        /** Type definitions for custom objects. */
        std::map<std::string, std::shared_ptr<ObjectType>> objectTypes;

        /** The parent scope, used to search for items. */
        std::weak_ptr<Scope> parent;

        friend class ScriptContext;
    };

}



