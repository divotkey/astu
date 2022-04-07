// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Item.h"
#include "ItemState.h"
#include "ItemStateInteger.h"
#include "ItemStateReal.h"
#include "ItemStateBool.h"
#include "ItemStateString.h"
#include "InterpreterError.h"
#include "IMemoryManager.h"

// C++ Standard Library
#include <cassert>
#include <cmath>

using namespace std;

#define TYPE_INDEX(a) static_cast<size_t>((a).state->GetType())

namespace velox {

    //std::shared_ptr<Item> Item::Create(std::unique_ptr<ItemState> state) {
    //    return std::shared_ptr<Item>(new Item(move(state)));
    //}

    std::shared_ptr<Item> Item::Copy() const {
        auto result = Create(state->Copy());

        // TODO test and reconsider if a shallow copy of sub-items is the right thing to do.
        result->subItems = result->subItems;

        return result;
    }

    void *Item::operator new(size_t count) {
        return gMemoryManager->Allocate(count);
    }

    void Item::operator delete(void *p) {
        gMemoryManager->Free(p);
    }

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

    const ItemType Item::relationalType[6][6] = {

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
                    ItemType::Undefined,

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
                    ItemType::Undefined,

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
                    ItemType::Undefined,

                    // Second type 'Other'
                    ItemType::Undefined
            },

            // First type 'String'
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

    bool Item::GetBooleanValue() const {
        return state->GetBooleanValue();
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
                throw InterpreterError("Undefined arithmetic operator between this types");

            case ItemType::Integer:
                return Item::Create(
                        std::make_unique<ItemStateInteger>(
                                ExecuteIntegerArithmetic(state->GetIntegerValue(), item.state->GetIntegerValue(), op)));

            case ItemType::Real:
                return Item::Create(
                        make_unique<ItemStateReal>(
                                ExecuteRealArithmetic(state->GetRealValue(), item.state->GetRealValue(), op)));

            case ItemType::Boolean:
                throw runtime_error(
                        "internal interpreter error: the result of an arithmetic operation "
                        "should never be of type boolean");

            case ItemType::String:
                if (op != ArithmeticOperator::ADD) {
                    throw InterpreterError("Operation not supported for strings.");
                }
                return Item::Create(make_unique<ItemStateString>(state->GetStringValue() + item.state->GetStringValue()));

            default:
                throw runtime_error("undefined result type for arithmetic operation");
        }
    }

    std::shared_ptr<Item> Item::ExecuteRelationalOperator(RelationalOperator op, const Item &item) const {
        // TODO look for custom operation function within this item for overloaded operators.

        // Get type used for the relational operation.
        ItemType opType = relationalType[TYPE_INDEX(*this)][TYPE_INDEX(item)];

        switch (opType) {
            case ItemType::Undefined:
                // Fall through

            case ItemType::Boolean:
                // Fall through

            case ItemType::String:
                // Fall through

            case ItemType::Other:
                throw InterpreterError("Undefined relational operator between this types");

            case ItemType::Integer:
                return Item::Create(make_unique<ItemStateBool>(
                        ExecuteIntegerRelational(state->GetIntegerValue(), item.state->GetIntegerValue(), op)));

            case ItemType::Real:
                return Item::Create(make_unique<ItemStateBool>(
                        ExecuteRealRelational(state->GetRealValue(), item.state->GetRealValue(), op)));

            default:
                throw runtime_error("Internal interpreter error: implementation of relational operator is flawed.");
        }
    }

    int Item::ExecuteIntegerArithmetic(int a, int b, ArithmeticOperator op) const {
        switch (op) {
            case ArithmeticOperator::ADD:
                return a + b;
            case ArithmeticOperator::SUB:
                return a - b;
            case ArithmeticOperator::MUL:
                return a * b;
            case ArithmeticOperator::DIV:
                return a / b;
            case ArithmeticOperator::MOD:
                return a % b;

            default:
                throw InterpreterError("Unknown arithmetic operator");
        }
    }

    double Item::ExecuteRealArithmetic(double a, double b, ArithmeticOperator op) const {
        switch (op) {
            case ArithmeticOperator::ADD:
                return a + b;
            case ArithmeticOperator::SUB:
                return a - b;
            case ArithmeticOperator::MUL:
                return a * b;
            case ArithmeticOperator::DIV:
                return a / b;
            case ArithmeticOperator::MOD:
                return std::fmod(a, b);

            default:
                throw InterpreterError("Unknown arithmetic operator");
        }
    }

    bool Item::ExecuteIntegerRelational(int a, int b, RelationalOperator op) const {
        switch (op) {

            case RelationalOperator::LESS_THAN:
                return a < b;

            case RelationalOperator::LESS_EQUAL:
                return a <= b;

            case RelationalOperator::GREATER_THAN:
                return a > b;

            case RelationalOperator::GREATER_EQUAL:
                return a >= b;

            case RelationalOperator::EQUAL:
                return a == b;

            case RelationalOperator::NOT_EQUAL:
                return a != b;

            default:
                throw runtime_error("Internal interpreter error: implementation of relational operator is flawed.");
        }
    }

    bool Item::ExecuteRealRelational(double a, double b, RelationalOperator op) const {
        switch (op) {

            case RelationalOperator::LESS_THAN:
                return a < b;

            case RelationalOperator::LESS_EQUAL:
                return a <= b;

            case RelationalOperator::GREATER_THAN:
                return a > b;

            case RelationalOperator::GREATER_EQUAL:
                return a >= b;

            case RelationalOperator::EQUAL:
                return a == b;

            case RelationalOperator::NOT_EQUAL:
                return a != b;

            default:
                throw runtime_error("Internal interpreter error: implementation of relational operator is flawed.");
        }

    }

}