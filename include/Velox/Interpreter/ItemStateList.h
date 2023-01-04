/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ItemStateWithSubItems.h"
#include "Graphics/Color.h"

// C++ Standard Library includes.
#include <vector>
#include <memory>

namespace velox {

    class ItemStateList : public ItemStateWithSubItems {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateList();

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;
        std::shared_ptr<Item> GetListElement(size_t idx, unsigned int lineNumber) override;
        void AppendListElement(std::shared_ptr<Item> elem) override;

    protected:
        // Inherited via ItemState
        bool Assign(Item &owner, const ItemState &rhs) override;

    private:
        /** The elements of this list. */
        std::vector<std::shared_ptr<Item>> elements;
    };

}



