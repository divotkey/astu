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

    void Scope::Clear() {
        items.clear();
    }

}