/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#include "Velox/Interpreter/ItemStateReal.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateReal::Copy() const {
        return make_unique<ItemStateReal>(value);
    }

    double ItemStateReal::GetRealValue(unsigned int lineNumber) const {
        return value;
    }

    int ItemStateReal::GetIntegerValue(unsigned int lineNumber) const {
        return static_cast<int>(value);
    }

    std::string ItemStateReal::GetStringValue(ScriptContext &sc) const {
        return to_string(value);
    }

    ItemType ItemStateReal::GetType() const {
        return ItemType::Real;
    }

    bool ItemStateReal::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Real)
            return false;

        // TODO get line number from somewheres
        value = rhs.GetRealValue(0);
        return true;
    }

    shared_ptr<Item> ItemStateReal::ExecuteUnaryMinus() const {
        return Item::CreateReal(-value);
    }
}