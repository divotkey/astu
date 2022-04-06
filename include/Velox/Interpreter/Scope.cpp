#include "Scope.h"

// C++ Standard Library
#include <cassert>
#include <stdexcept>

using namespace std;

namespace velox {

    void Scope::AddItem(const string &name, std::shared_ptr<Item> item) {
        assert(!HasItem(name));
        items[name] = item;
    }

    bool Scope::HasItem(const string &name) const {
        return items.find(name) != items.end();
    }

    std::shared_ptr<Item> Scope::FindItem(const string &name) {
        auto it = items.find(name);
        if (it != items.end()) {
            return it->second;
        }

        return nullptr;
    }

    std::shared_ptr<const Item> Scope::FindItem(const string &name) const {
        auto it = items.find(name);
        if (it != items.end()) {
            return it->second;
        }

        return nullptr;
    }

    Item &Scope::GetItem(const string &name) {
        auto result = FindItem(name);
        if (!result) {
            throw std::logic_error("Unknown item '" + name + "'");
        }

        return *result;
    }

    const Item &Scope::GetItem(const string &name) const {
        auto result = FindItem(name);
        if (!result) {
            throw std::logic_error("Unknown item '" + name + "'");
        }

        return *result;
    }

    void Scope::Clear() {
        items.clear();
    }

}