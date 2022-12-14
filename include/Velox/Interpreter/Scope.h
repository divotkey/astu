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
         *
         * @param localBorder   whether this scope marks the border of locality of variables
         */
        Scope(bool localBorder = false) : localBorder(localBorder) {}

        /**
         * Add anonymous item to current scope.
         *
         * @param item  the item to add
         */
        void AddItem(std::shared_ptr<Item> item);

        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        bool HasItem(const std::string& name) const;
        std::shared_ptr<Item> FindItem(const std::string& name);
        std::shared_ptr<const Item> FindItem(const std::string& name) const;

        void Clear();
        bool IsLocalBorder() const {
            return localBorder;
        }

    private:
        /** List of items associated with unique names. */
        std::map<std::string, std::shared_ptr<Item>> items;

        /** Holds items without names. */
        std::vector<std::shared_ptr<Item>> anonymousItems;

        /** Whether this scope marks the border of locality of variables. */
        bool localBorder;
    };

}



