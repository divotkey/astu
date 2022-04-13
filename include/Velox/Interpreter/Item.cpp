// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Item.h"
#include "ItemStateInteger.h"
#include "ItemStateReal.h"
#include "ItemStateBool.h"
#include "ItemStateString.h"
#include "ItemStateReference.h"
#include "InterpreterError.h"
#include "IMemoryManager.h"

// C++ Standard Library
#include <cassert>
#include <cmath>

using namespace std;

#define TYPE_INDEX(a) static_cast<size_t>((a).state->GetType())

namespace velox {

    const string Item::arithmeticOperatorName[] = {"operator+", "operator-", "operator*", "operator/", "operator%"};

    const ItemType Item::arithmeticResult[6][6] = {
            // First type 'Undefined'
            {{ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined},},

            // First type 'Integer'
            {{ItemType::Undefined}, {ItemType::Integer},   {ItemType::Real},      {ItemType::Undefined}, {ItemType::String},    {ItemType::Undefined},},

            // First type 'Real'
            {{ItemType::Undefined}, {ItemType::Real},      {ItemType::Real},      {ItemType::Undefined}, {ItemType::String},    {ItemType::Undefined},},

            // First type 'Boolean'
            {{ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::String},    {ItemType::Undefined},},

            // First type 'String'
            {{ItemType::String},    {ItemType::String},    {ItemType::String},    {ItemType::String},    {ItemType::String},    {ItemType::String},},

            // First type 'Other'
            {{ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined},},
    };

    const ItemType Item::relationalType[6][6] = {
            // First type 'Undefined'
            {{ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined},},

            // First type 'Integer'
            {{ItemType::Undefined}, {ItemType::Integer},   {ItemType::Real},      {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined},},

            // First type 'Real'
            {{ItemType::Undefined}, {ItemType::Real},      {ItemType::Real},      {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined},},

            // First type 'Boolean'
            {{ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined},},

            // First type 'String'
            {{ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::String},    {ItemType::Undefined},},

            // First type 'Other'
            {{ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined}, {ItemType::Undefined},},
    };

    std::shared_ptr<Item> Item::Create(std::shared_ptr<Item> item) {
        if (item->state->GetType() == ItemType::Other) {
            return Item::Create(make_unique<ItemStateReference>(item));
        }
        return Item::Create(item->state->Copy());
    }

    std::shared_ptr<Item> Item::Copy() const {
        auto result = Create(state->Copy());
        state->CopyItems(*result);
        return result;
    }

    void *Item::operator new(size_t count) {
        return gMemoryManager->Allocate(count);
    }

    void Item::operator delete(void *p) {
        gMemoryManager->Free(p);
    }

    void Item::Assign(std::shared_ptr<Item> rhs) {
        state->Assign(*this, rhs);
    }

    std::shared_ptr<Item>
    Item::CallAsFunction(ScriptContext &sc, InterpreterActualParameterList &parameters, unsigned int lineNumber) {
        auto parent = GetParent();
        if (parent) {
            sc.AddItem("this", Item::Create(make_unique<ItemStateReference>(parent)));
            parent->AddItemsToScope(sc);
        }
        return state->CallAsFunction(sc, parameters, lineNumber);
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

    std::string Item::GetStringValue(ScriptContext &sc) const {
        return state->GetStringValue(sc);
    }

    void Item::AddItem(const string &name, std::shared_ptr<Item> item) {
        if (state->AddItem(name, item)) {
            item->parent = shared_from_this();
        }
    }

    bool Item::HasItem(const string &name) const {
        return state->FindItem(name) != nullptr;
    }

    std::shared_ptr<Item> Item::FindItem(const string &name) {
        return state->FindItem(name);
    }

    Item &Item::GetItem(const string &name) {
        auto result = FindItem(name);
        if (!result) {
            throw std::logic_error("Unknown sub-item '" + name + "'");
        }

        return *result;
    }

    std::shared_ptr<Item> Item::ExecuteArithmeticOperator(ScriptContext &sc, ArithmeticOperator op, std::shared_ptr<Item> item) const {
        // TODO look for custom operation function within this item for overloaded operators.

        auto opFunc = state->FindItem(arithmeticOperatorName[static_cast<int>(op)]);
        if (opFunc) {
            InterpreterItemParameterList params;
            params.AddParameter(item);
            return opFunc->CallAsFunction(sc, params, 0);
        }

        // Get result type.
        ItemType resultType = arithmeticResult[TYPE_INDEX(*this)][TYPE_INDEX(*item)];

        switch (resultType) {
            case ItemType::Undefined:
                // fall through

            case ItemType::Other:
                throw InterpreterError("Undefined arithmetic operator between this types");

            case ItemType::Integer:
                return Item::Create(
                        std::make_unique<ItemStateInteger>(
                                ExecuteIntegerArithmetic(state->GetIntegerValue(), item->state->GetIntegerValue(), op)));

            case ItemType::Real:
                return Item::Create(
                        make_unique<ItemStateReal>(
                                ExecuteRealArithmetic(state->GetRealValue(), item->state->GetRealValue(), op)));

            case ItemType::Boolean:
                throw runtime_error(
                        "internal interpreter error: the result of an arithmetic operation "
                        "should never be of type boolean");

            case ItemType::String:
                if (op != ArithmeticOperator::ADD) {
                    throw InterpreterError("Operation not supported for strings.");
                }
                return Item::Create(make_unique<ItemStateString>(state->GetStringValue(sc) + item->state->GetStringValue(
                        sc)));

            default:
                throw runtime_error("undefined result type for arithmetic operation");
        }
    }

    std::shared_ptr<Item> Item::ExecuteRelationalOperator(ScriptContext &sc, RelationalOperator op, const Item &item) const {
        // TODO look for custom operation function within this item for overloaded operators.

        if (state->GetType() == ItemType::Undefined || item.state->GetType() == ItemType::Undefined) {
            return Item::Create(make_unique<ItemStateBool>(state->GetType() == item.state->GetType()));
        }

        // Get type primary type for the relational operation.
        switch (relationalType[TYPE_INDEX(*this)][TYPE_INDEX(item)]) {
            case ItemType::Boolean:
                // Fall through

            case ItemType::Other:
                throw InterpreterError("Undefined relational operator between this types");

            case ItemType::String:
                return Item::Create(make_unique<ItemStateBool>(
                        ExecuteStringRelational(state->GetStringValue(sc), item.state->GetStringValue(sc), op)));

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

    std::shared_ptr<Item> Item::GetParent() {
        return state->GetParent(*this);
    }

    void Item::AddItemsToScope(ScriptContext &sc) const {
        state->AddItemsToScope(sc);
    }

    bool Item::ExecuteStringRelational(const string &a, const string &b, RelationalOperator op) const {
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

    std::shared_ptr<Item> Item::GetReferencedItem() {
        return state->GetReferencedItem();
    }

    void Item::SetData(std::shared_ptr<ItemData> data) {
        state->SetData(data);
    }

    std::shared_ptr<ItemData> Item::GetData() {
        return state->GetData();
    }

}