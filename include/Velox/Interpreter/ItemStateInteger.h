#pragma once

#include "ItemState.h"

namespace velox {

    class ItemStateInteger : public ItemState {
    public:

        /**
         * Constructor.
         * @param value the value of this state
         */
        ItemStateInteger(int value) : value(value) {}

        ItemStateInteger( const ItemStateInteger& ) = delete; // non construction-copyable
        ItemStateInteger& operator=( const ItemStateInteger& ) = delete; // non copyable

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
        int value;
    };

}


