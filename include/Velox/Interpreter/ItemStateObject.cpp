// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "ItemStateObject.h"
#include "Item.h"
#include "ItemStateReference.h"
#include "InterpreterActualParameterList.h"

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

}
