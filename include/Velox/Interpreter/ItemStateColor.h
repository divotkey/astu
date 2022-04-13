#pragma once

// Local includes
#include "ItemState.h"
#include "Graphics/Color.h"

namespace velox {

    class ItemStateColor : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateColor(const astu::Color4d &value);

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override;
        int GetIntegerValue() const override;
        std::string GetStringValue(ScriptContext &sc) const override;
        const astu::Color4d & GetColorValue() const override;

        std::shared_ptr<Item> FindItem(const std::string &name) override;

        ItemType GetType() const override;

    protected:
        // Inherited via ItemState
        bool Assign(Item &owner, const ItemState &rhs) override;

    private:
        /** The value of this state. */
        astu::Color4d value;
    };

}



