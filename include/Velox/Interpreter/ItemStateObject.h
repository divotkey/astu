/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ItemStateWithSubItems.h"

// C++ Standard Library includes
#include <map>

namespace velox {

    class ItemStateObject : public ItemStateWithSubItems {
    public:

        /** Constructor. */
        ItemStateObject();

        // Inherited via ItemState
        std::unique_ptr <ItemState> Copy() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;
        void SetData(std::shared_ptr<ItemData> data) override;
        std::shared_ptr<ItemData> GetData() override;

    protected:
        bool Assign(Item &owner, const ItemState &rhs) override;

    private:
        /** Additional data. */
        std::shared_ptr<ItemData> data;
    };

}


