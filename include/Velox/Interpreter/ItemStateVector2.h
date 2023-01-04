/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ItemStateWithSubItems.h"
#include "Math/Vector2.h"
#include "Graphics/Color.h"

namespace velox {

    class ItemStateVector2 : public ItemStateWithSubItems {
    public:

        /**
         * Constructor.
         *
         * @param value the value of this state
         */
        ItemStateVector2(const astu::Vector2d &value);

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;
        const astu::Vector2d &GetVector2Value() const override;

        std::shared_ptr<Item> ExecuteUnaryMinus() const override;

    protected:

        // Inherited via ItemState
        bool Assign(Item &owner, const ItemState &rhs) override;

    private:
        /** The value of this state. */
        astu::Vector2d value;
    };

}



