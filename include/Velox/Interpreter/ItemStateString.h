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

    class ItemStateString : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        explicit ItemStateString(const std::string& value);

        /**
         * Destructor.
         */
        ~ItemStateString() override;

        ItemStateString( const ItemStateString& ) = delete; // non construction-copyable
        ItemStateString& operator=( const ItemStateString& ) = delete; // non copyable

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        ItemType GetType() const override;

    private:
        /** The value of this state. */
        std::string value;

        // Alternative implementation using memory manager.
        //char *value;
    };

}



