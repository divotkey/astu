// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "ItemStateReference.h"
#include "Item.h"

using namespace std;

namespace velox {

    ItemStateReference::ItemStateReference(std::shared_ptr<Item> inValue)  {
        value = inValue->GetReferencedItem();
        if (!value)
            value = inValue;
    }

    std::shared_ptr<Item>
    ItemStateReference::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters,
                                       unsigned int lineNumber)
    {
        return value->state->CallAsFunction(sc, parameters, 0);
    }

    unique_ptr<ItemState> ItemStateReference::Copy() const {
        return value->state->Copy();
    }

    double ItemStateReference::GetRealValue() const {
        return value->state->GetRealValue();
    }

    int ItemStateReference::GetIntegerValue() const {
        return value->state->GetIntegerValue();
    }

    std::string ItemStateReference::GetStringValue(ScriptContext &sc) const {
        return value->state->GetStringValue(sc);
    }

    ItemType ItemStateReference::GetType() const {
        return value->state->GetType();
    }

    std::shared_ptr<Item> ItemStateReference::FindItem(const string &name) const {
        return value->state->FindItem(name);
    }

    bool ItemStateReference::AddItem(const string &name, std::shared_ptr<Item> item) {
        return value->state->AddItem(name, item);
    }

    std::shared_ptr<Item> ItemStateReference::GetParent(Item &context) {
        return value->GetParent();
    }

    std::shared_ptr<Item> ItemStateReference::GetReferencedItem() {
        return value;
    }

    void ItemStateReference::SetData(std::shared_ptr<ItemData> data) {
        value->SetData(data);
    }

    std::shared_ptr<ItemData> ItemStateReference::GetData() {
        return value->GetData();
    }

}
