/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
 */

#pragma once

#include "ItemState.h"

namespace velox {

    class ItemStateReal : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateReal(double value = 0) : value(value) {}

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        double GetRealValue(unsigned int lineNumber) const override;
        int GetIntegerValue(unsigned int lineNumber) const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;

        std::shared_ptr<Item> ExecuteUnaryMinus() const override;

    protected:
        // Inherited via ItemState
        bool Assign(Item &owner, const ItemState &rhs) override;

    private:
        /** The value of this state. */
        double value;
    };

}



