/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ItemState.h"

// C++ Standard Library includes
#include <map>

namespace velox {

    class ItemStateWithSubItems : public ItemState {
    public:

        // Inherited via ItemState
        std::shared_ptr<Item> FindItem(const std::string &name) const override;
        bool AddItem(const std::string &name, std::shared_ptr<Item> item) override;
        void CopyItems(Item& target) override;
        void AddItemsToScope(Scope &scope) override;

    private:
        /** List of sub-items associated with unique names. */
        std::map<std::string, std::shared_ptr<Item>> subItems;
    };

}


