#pragma once

#include <memory>
#include <string>
#include <deque>

namespace velox {

    // Forward declaration.
    class Item;
    class Scope;

    class ScriptContext {

    public:
        void PushScope(std::shared_ptr<Scope> scope);
        void PopScope();

        bool HasItem(const std::string &name) const;
        Item& GetItem(const std::string &name);
        const Item& GetItem(const std::string &name) const;
        std::shared_ptr<Item> FindItem(const std::string &name);
        std::shared_ptr<const Item> FindItem(const std::string &name) const;
        void AddItem(const std::string& name, std::shared_ptr<Item> item);
        void Clear();

    private:
        /** The hierarchy of scopes. */
        std::deque<std::shared_ptr<Scope>> scopes;
    };

}


