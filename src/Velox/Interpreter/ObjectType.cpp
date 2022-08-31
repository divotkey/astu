// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/ObjectType.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateObject.h"
#include "Velox/Interpreter/InterpreterError.h"

#include <cassert>

using namespace std;

namespace velox {

    std::shared_ptr<Item> ObjectType::CreateObject(ScriptContext &sc) {
        auto result = Item::Create(make_unique<ItemStateObject>());

        for (auto it : items) {
            result->AddItem(it.first, it.second->Copy());
        }

        return result;
    }

    bool ObjectType::HasItem(const string &name) const {
        return items.find(name) != items.end();
    }

    void ObjectType::AddItem(const string &name, std::shared_ptr<Item> item) {
        assert(!HasItem(name));
        items[name] = item;
    }

}