// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "ItemStateWithSubItems.h"
#include "Item.h"
#include "ItemStateReference.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace velox {

    shared_ptr<Item> ItemStateWithSubItems::FindItem(const string &name) {
        auto it = subItems.find(name);
        if (it != subItems.end())
            return it->second;

        return nullptr;
    }

    bool ItemStateWithSubItems::AddItem(const string &name, std::shared_ptr<Item> item) {
        assert(FindItem(name) == nullptr);
        subItems[name] = item;
        return true;
    }

    void ItemStateWithSubItems::CopyItems(Item &target) {
        for (auto it : subItems) {
            target.AddItem(it.first, it.second->Copy());
        }
    }

    void ItemStateWithSubItems::AddItemsToScope(ScriptContext &sc) {
        for (auto it: subItems) {
            sc.AddItem(it.first, Item::Create(make_unique<ItemStateReference>(it.second)));
        }

        ItemState::AddItemsToScope(sc);
    }

}
