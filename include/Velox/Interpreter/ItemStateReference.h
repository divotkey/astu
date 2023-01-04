/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

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
         *
         * @param value the value of this state
         */
        ItemStateReference(std::shared_ptr<Item> value);

        // Inherited via ItemState
        std::shared_ptr<Item>
        CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, std::shared_ptr<Scope> memberScope, unsigned int lineNumber) override;
        std::unique_ptr<ItemState> Copy() const override;
        double GetRealValue(unsigned int lineNumber) const override;
        int GetIntegerValue(unsigned int lineNumber) const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;
        std::shared_ptr<Item> FindItem(const std::string &name) const override;
        bool AddItem(const std::string &name, std::shared_ptr<Item> item) override;
        std::shared_ptr<Item> GetParent(Item &context) override;
        bool IsReference() const override;
        std::shared_ptr<Item> GetReferencedItem() override;
        void SetData(std::shared_ptr<ItemData> data) override;
        std::shared_ptr<ItemData> GetData() override;

    private:
        /** The value of this state. */
        std::shared_ptr<Item> value;
    };

}


