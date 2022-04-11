#pragma once

#include "ItemState.h"

namespace velox {

    class ItemStateBool : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateBool(bool value) : value(value) {}

        ItemStateBool( const ItemStateBool& ) = delete; // non construction-copyable
        ItemStateBool& operator=( const ItemStateBool& ) = delete; // non copyable

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        double GetRealValue() const override;
        int GetIntegerValue() const override;
        bool GetBooleanValue() const override;
        std::string GetStringValue() const override;
        ItemType GetType() const override;

    protected:
        // Inherited via ItemState
        bool Assign(Item &owner, const ItemState &rhs) override;

    private:
        /** The value of this state. */
        bool value;
    };

}



