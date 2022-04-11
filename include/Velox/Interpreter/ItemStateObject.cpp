// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "ItemStateObject.h"
#include "Item.h"
#include "ItemStateReference.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateObject::Copy() const {
        return make_unique<ItemStateObject>();
    }

    string ItemStateObject::GetStringValue() const {
        return "undefined";
    }

    ItemType ItemStateObject::GetType() const {
        return ItemType::Other;
    }

    shared_ptr<Item> ItemStateObject::FindItem(const string &name) {
        auto it = subItems.find(name);
        if (it != subItems.end())
            return it->second;

        return nullptr;
    }

    bool ItemStateObject::AddItem(const string &name, std::shared_ptr<Item> item) {
        assert(FindItem(name) == nullptr);
        subItems[name] = item;
        return true;
    }

    void ItemStateObject::CopyItems(Item &target) {
        for (auto it : subItems) {
            target.AddItem(it.first, it.second->Copy());
        }
    }

    void ItemStateObject::AddItemsToScope(ScriptContext &sc) {
        for (auto it : subItems) {
            sc.AddItem(it.first, Item::Create(make_unique<ItemStateReference>(it.second)));
        }

        ItemState::AddItemsToScope(sc);
    }

}
