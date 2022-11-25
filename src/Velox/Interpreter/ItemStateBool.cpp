// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/ItemStateBool.h"

// C++ Standard Library includes
#include <string>

using namespace std;

namespace velox {

    unique_ptr<ItemState> ItemStateBool::Copy() const {
        return make_unique<ItemStateBool>(value);
    }

    double ItemStateBool::GetRealValue(unsigned int lineNumber) const {
        return static_cast<double>(value);
    }

    int ItemStateBool::GetIntegerValue(unsigned int lineNumber) const {
        return static_cast<int>(value);
    }

    bool ItemStateBool::GetBooleanValue(unsigned int lineNumber) const {
        return value;
    }

    std::string ItemStateBool::GetStringValue(ScriptContext &sc) const {
        return (value ? "true" : "false");
    }

    ItemType ItemStateBool::GetType() const {
        return ItemType::Boolean;
    }

    bool ItemStateBool::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Boolean)
            return false;

        // TODO get line number from somewheres
        value = rhs.GetBooleanValue(0);
        return true;
    }
}