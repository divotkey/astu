#pragma once

#include <string>
#include <memory>
#include <map>

namespace velox {

    // Forward declaration.
    class Item;

    class Scope {
    public:

        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        bool HasItem(const std::string& name) const;
        std::shared_ptr<Item> FindItem(const std::string& name);
        std::shared_ptr<const Item> FindItem(const std::string& name) const;
        Item &GetItem(const std::string& name);
        const Item &GetItem(const std::string& name) const;
        void Clear();

    private:
        /** List of items associated with unique names. */
        std::map<std::string, std::shared_ptr<Item>> items;
    };

}



