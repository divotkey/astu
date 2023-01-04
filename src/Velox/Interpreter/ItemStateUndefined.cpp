/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/ItemStateUndefined.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateUndefined::Copy() const {
        return make_unique<ItemStateUndefined>();
    }

    std::string ItemStateUndefined::GetStringValue(ScriptContext &sc) const {
        return "undefined";
    }

    ItemType ItemStateUndefined::GetType() const {
        return ItemType::Undefined;
    }

    bool ItemStateUndefined::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() == ItemType::Undefined)
            return true;

        return false;
    }

}
