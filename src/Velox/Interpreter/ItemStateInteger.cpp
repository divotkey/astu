/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/ItemStateInteger.h"
#include "Velox/Interpreter/Item.h"

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateInteger::Copy() const {
        return make_unique<ItemStateInteger>(value);
    }

    double ItemStateInteger::GetRealValue(unsigned int lineNumber) const {
        return static_cast<double>(value);
    }

    int ItemStateInteger::GetIntegerValue(unsigned int lineNumber) const {
        return value;
    }

    std::string ItemStateInteger::GetStringValue(ScriptContext &sc) const {
        return to_string(value);
    }

    ItemType ItemStateInteger::GetType() const {
        return ItemType::Integer;
    }

    bool ItemStateInteger::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Integer)
            return false;

        // TODO get line number from somewheres
        value = rhs.GetIntegerValue(0);
        return true;
    }

    shared_ptr<Item> ItemStateInteger::ExecuteUnaryMinus() const {
        return Item::CreateInteger(-value);
    }

    //void *ItemStateInteger::operator new(size_t count) {
    //    return gMemoryManager->Allocate(count);
    //}
    //
    //void ItemStateInteger::operator delete(void *p) {
    //    gMemoryManager->Free(p);
    //}

}