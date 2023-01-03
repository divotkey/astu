/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/Scope.h"

// C++ Standard Library
#include <cassert>
#include <stdexcept>

using namespace std;

namespace velox {

    void Scope::AddItem(std::shared_ptr<Item> item) {
        anonymousItems.push_back(item);
    }

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

        auto pParent = parent.lock();
        if (pParent) {
            return pParent->FindItem(name);
        }

        return nullptr;
    }

    std::shared_ptr<const Item> Scope::FindItem(const string &name) const {
        auto it = items.find(name);
        if (it != items.end()) {
            return it->second;
        }

        auto pParent = parent.lock();
        if (pParent) {
            return pParent->FindItem(name);
        }

        return nullptr;
    }

    void Scope::Clear() {
        items.clear();
        anonymousItems.clear();
    }

} // end of namespace