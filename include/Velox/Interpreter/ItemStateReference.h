#pragma once

#include "ItemState.h"

namespace velox {

    // Forward declaration
    class Item;

    class ItemStateReference : public ItemState {

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateReference(std::shared_ptr<Item> value) : value(value) {}

        // Inherited via ItemState
        std::shared_ptr<Item> CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters) override;
        std::unique_ptr<ItemState> Copy() const override;
        double GetRealValue() const override;
        int GetIntegerValue() const override;
        std::string GetStringValue() const override;
        ItemType GetType() const override;

    private:
        /** The value of this state. */
        std::shared_ptr<Item> value;
    };

}


