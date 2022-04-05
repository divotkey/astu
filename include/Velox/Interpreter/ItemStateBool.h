#pragma once

#include "ItemState.h"

namespace velox {

    class ItemStateBool : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateBool(bool value = 0) : value(value) {}

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        double GetRealValue() const override;
        int GetIntegerValue() const override;
        bool GetBooleanValue() const override;
        std::string GetStringValue() const override;
        ItemType GetType() const override;

    private:
        /** The value of this state. */
        bool value;
    };

}



