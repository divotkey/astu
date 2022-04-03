#pragma once

#include "ItemState.h"

namespace velox {

    class ItemStateUndefined : public ItemState {
    public:

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        std::string GetStringValue() const override;
        ItemType GetType() const override;

    };

}


