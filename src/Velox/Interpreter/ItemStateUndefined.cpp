// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

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
