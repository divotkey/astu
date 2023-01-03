/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
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
         * Removes all items in this scope.
         */
        void Clear();

    private:
        /** List of items associated with unique names. */
        std::map<std::string, std::shared_ptr<Item>> items;

        /** Holds items without names. */
        std::vector<std::shared_ptr<Item>> anonymousItems;

        /** The parent scope, used to search for items. */
        std::weak_ptr<Scope> parent;

        friend class ScriptContext;
    };

}



