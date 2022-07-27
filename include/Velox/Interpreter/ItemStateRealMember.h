#pragma once

// Local includes
#include "ItemState.h"
#include "ItemStateReal.h"

namespace velox {

    class ItemStateRealMember : public ItemState {
    public:

        /**
         * Constructor
         *
         * @param value reference to the real-member of this state
         */
        ItemStateRealMember(double &value) : value(value) {}

        // Inherited via ItemState
        std::unique_ptr<ItemState> Copy() const override {
            return std::make_unique<ItemStateReal>(value);
        }

        double GetRealValue(unsigned int lineNumber) const override {
            return value;
        }

        int GetIntegerValue(unsigned int lineNumber) const override {
            return static_cast<int>(value);
        }

        std::string GetStringValue(ScriptContext &sc) const override {
            return std::to_string(value);
        }

        bool Assign(Item &owner, const ItemState &rhs) override {
            value = rhs.GetRealValue(0);
            return true;
        }

        ItemType GetType() const override {
            return ItemType::Real;
        }

    private:
        double &value;
    };

}