// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#pragma once

// Local includes
#include "ItemState.h"

namespace velox {

    // Forward declaration
    class Item;

    class ItemStateReference : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        explicit ItemStateReference(std::shared_ptr<Item> value);

        // Inherited via ItemState
        std::shared_ptr<Item>
        CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, unsigned int lineNumber) override;
        std::unique_ptr<ItemState> Copy() const override;
        double GetRealValue() const override;
        int GetIntegerValue() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;
        std::shared_ptr<Item> FindItem(const std::string &name) override;
        bool AddItem(const std::string &name, std::shared_ptr<Item> item) override;
        std::shared_ptr<Item> GetParent(Item &context) override;
        std::shared_ptr<Item> GetReferencedItem() override;
        void SetData(std::shared_ptr<ItemData> data) override;
        std::shared_ptr<ItemData> GetData() override;

    private:
        /** The value of this state. */
        std::shared_ptr<Item> value;
    };

}


