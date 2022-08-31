// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/ItemStateObject.h"
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateReference.h"
#include "Velox/Interpreter/InterpreterActualParameterList.h"

using namespace std;

#define TO_STRING_METHOD "ToString"

namespace velox {

    ItemStateObject::ItemStateObject() : data(nullptr) {
        // Intentionally left empty.
    }

    unique_ptr<ItemState> ItemStateObject::Copy() const {
        return make_unique<ItemStateObject>();
    }

    std::string ItemStateObject::GetStringValue(ScriptContext &sc) const {
        auto funcItem = FindItem(TO_STRING_METHOD);
        if (!funcItem) {
            return ItemState::GetStringValue(sc);
        }
        // TODO improve this memory usage.
        InterpreterNoParameterList noParams;
        return funcItem->CallAsFunction(sc, noParams, 0)->GetStringValue(sc);
    }

    ItemType ItemStateObject::GetType() const {
        return ItemType::Other;
    }


    void ItemStateObject::SetData(std::shared_ptr<ItemData> inData) {
        data = inData;
    }

    std::shared_ptr<ItemData> ItemStateObject::GetData() {
        return data;
    }

    bool ItemStateObject::Assign(Item &owner, const ItemState &rhs) {
        if (this == &rhs)
            return true;

        return false;
    }

}
