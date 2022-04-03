#pragma once

#include "ItemState.h"

namespace velox {

    class ItemStateInteger : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateInteger(int value = 0) : value(value) {}

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        double GetRealValue() const override;
        int GetIntegerValue() const override;
        std::string GetStringValue() const override;
        ItemType GetType() const override;

    private:
        /** The value of this state. */
        int value;
    };

}



