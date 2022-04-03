#include "Item.h"
#include "ItemState.h"
#include "ItemStateInteger.h"
#include "ItemStateReal.h"
#include "InterpreterException.h"

#include <cassert>

using namespace std;

#define TYPE_INDEX(a) static_cast<size_t>((a).state->GetType())

namespace velox {

    const ItemType Item::arithmeticResult[6][6] = {

            // First type 'Undefined'
            {
                    // Second type 'Undefined'
                    ItemType::Undefined,

                    // Second type 'Integer'
                    ItemType::Undefined,

                    // Second type 'Real'
                    ItemType::Undefined,

                    // Second type 'Boolean'
                    ItemType::Undefined,

                    // Second type 'String'
                    ItemType::Undefined,

                    // Second type 'Other'
                    ItemType::Undefined
            },

            // First type 'Integer'
            {
                    // Second type 'Undefined'
                    ItemType::Undefined,

                    // Second type 'Integer'
                    ItemType::Integer,

                    // Second type 'Real'
                    ItemType::Real,

                    // Second type 'Boolean'
                    ItemType::Undefined,

                    // Second type 'String'
                    ItemType::String,

                    // Second type 'Other'
                    ItemType::Undefined
            },

            // First type 'Real'
            {
                    // Second type 'Undefined'
                    ItemType::Undefined,

                    // Second type 'Integer'
                    ItemType::Real,

                    // Second type 'Real'
                    ItemType::Real,

                    // Second type 'Boolean'
                    ItemType::Undefined,

                    // Second type 'String'
                    ItemType::String,

                    // Second type 'Other'
                    ItemType::Undefined
            },

            // First type 'Boolean'
            {
                    // Second type 'Undefined'
                    ItemType::Undefined,

                    // Second type 'Integer'
                    ItemType::Undefined,

                    // Second type 'Real'
                    ItemType::Undefined,

                    // Second type 'Boolean'
                    ItemType::Undefined,

                    // Second type 'String'
                    ItemType::String,

                    // Second type 'Other'
                    ItemType::Undefined
            },

            // First type 'String'
            {
                    // Second type 'Undefined'
                    ItemType::String,

                    // Second type 'Integer'
                    ItemType::String,

                    // Second type 'Real'
                    ItemType::String,

                    // Second type 'Boolean'
                    ItemType::String,

                    // Second type 'String'
                    ItemType::String,

                    // Second type 'Other'
                    ItemType::Undefined
            },

            // First type 'Other'
            {
                    // Second type 'Undefined'
                    ItemType::Undefined,

                    // Second type 'Integer'
                    ItemType::Undefined,

                    // Second type 'Real'
                    ItemType::Undefined,

                    // Second type 'Boolean'
                    ItemType::Undefined,

                    // Second type 'String'
                    ItemType::Undefined,

                    // Second type 'Other'
                    ItemType::Undefined
            },
    };

    Item::Item(std::unique_ptr<ItemState> state) : state(move(state)) {
        // Intentionally left empty.
    }

    void Item::Assign(const Item &other) {
        state = other.state->Copy();
    }

    std::shared_ptr<Item> Item::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList& parameters) {
        return state->CallAsFunction(sc, parameters);
    }

    double Item::GetRealValue() const {
        return state->GetRealValue();
    }

    int Item::GetIntegerValue() const {
        return state->GetIntegerValue();
    }

    std::string Item::GetStringValue() const {
        return state->GetStringValue();
    }

    void Item::AddItem(const string &name, std::shared_ptr<Item> item) {
        assert(!HasItem(name));
        subItems[name] = item;
    }

    bool Item::HasItem(const string &name) const {
        return subItems.find(name) == subItems.end();
    }

    std::shared_ptr<Item> Item::FindItem(const string &name) {
        auto it = subItems.find(name);
        if (it != subItems.end()) {
            return it->second;
        }

        return nullptr;
    }

    std::shared_ptr<const Item> Item::FindItem(const string &name) const {
        auto it = subItems.find(name);
        if (it != subItems.end()) {
            return it->second;
        }

        return nullptr;
    }

    Item &Item::GetItem(const string &name) {
        auto result = FindItem(name);
        if (!result) {
            throw std::logic_error("Unknown sub item '" + name + "'");
        }

        return *result;
    }

    const Item &Item::GetItem(const string &name) const {
        auto result = FindItem(name);
        if (!result) {
            throw std::logic_error("Unknown sub item '" + name + "'");
        }

        return *result;
    }

    std::shared_ptr<Item> Item::ExecuteArithmeticOperator(ArithmeticOperator op, const Item &item) const {
        // TODO look for custom operation function within this item for overloaded operators.

        // Get result type.
        ItemType resultType = arithmeticResult[TYPE_INDEX(*this)][TYPE_INDEX(item)];

        switch (resultType) {
            case ItemType::Undefined:
                // fall through

            case ItemType::Other:
                throw InterpreterException("Undefined arithmetic operator between this types");

            case ItemType::Integer:
                return make_shared<Item>(
                        make_unique<ItemStateInteger>(state->GetIntegerValue() + item.state->GetIntegerValue()));

            case ItemType::Real:
                return make_shared<Item>(
                        make_unique<ItemStateReal>(state->GetRealValue() + item.state->GetRealValue()));

            case ItemType::Boolean:
                throw runtime_error(
                        "internal interpreter error: the result of an arithmetic operation "
                        "should never be of type boolean");

            case ItemType::String:
                throw runtime_error("string result of arithmetic operation not implemented");

            default:
                throw runtime_error("undefined result type for arithmetic operation");
        }

    }

}