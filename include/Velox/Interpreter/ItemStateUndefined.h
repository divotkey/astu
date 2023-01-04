/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include "ItemState.h"

namespace velox {

    class ItemStateUndefined : public ItemState {
    public:

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;

    protected:
        // Inherited via ItemState
        bool Assign(Item &owner, const ItemState &rhs) override;

    };

}


