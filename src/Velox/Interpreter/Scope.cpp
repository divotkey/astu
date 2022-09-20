// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
        if (HasItem(name)) {
            int x = 12;
        }
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
        anonymousItems.clear();
    }



}