// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemStateInteger.h"
#include "Velox/Interpreter/ItemStateBool.h"
#include "Velox/Interpreter/ItemStateReference.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/IMemoryManager.h"

// C++ Standard Library
#include <cassert>

using namespace std;

#define TYPE_INDEX(a) static_cast<size_t>((a).state->GetType())

namespace velox {

    const string Item::arithmeticOperatorName[] = {"operator+", "operator-", "operator*", "operator/", "operator%"};

    const string Item::relationalOperatorName[] = {"operator>", "operator>=", "operator<=", "operator</>", "operator==",
                                                   "operator!="};

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

        // This local scope will contain class members etc.
        shared_ptr<Scope> memberScope;
        if (parent) {
            memberScope = make_shared<Scope>();
            memberScope->AddItem("this", Item::Create(make_unique<ItemStateReference>(parent)));
            parent->AddItemsToScope(*memberScope);
        }

        return state->CallAsFunction(sc, parameters, memberScope, lineNumber);
    }

    double Item::GetRealValue(unsigned int lineNumber) const {
        return state->GetRealValue(lineNumber);
    }

    int Item::GetIntegerValue(unsigned int lineNumber) const {
        return state->GetIntegerValue(lineNumber);
    }

    bool Item::GetBooleanValue(unsigned int lineNumber) const {
        return state->GetBooleanValue(lineNumber);
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

    std::shared_ptr<Item> Item::FindItem(const string &name) const {
        return state->FindItem(name);
    }

    Item &Item::GetItem(const string &name) {
        auto result = FindItem(name);
        if (!result) {
            throw std::logic_error("Unknown sub-item '" + name + "'");
        }

        return *result;
    }

    std::shared_ptr<Item> Item::ExecuteUnaryMinus() const {
        return state->ExecuteUnaryMinus();
    }

    std::shared_ptr<Item> Item::GetParent() {
        return state->GetParent(*this);
    }

    void Item::AddItemsToScope(Scope &scope) const {
        state->AddItemsToScope(scope);
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

    ItemType Item::GetType() const {
        return state->GetType();
    }

    const astu::Color4d &Item::GetColorValue() const {
        return state->GetColorValue();
    }

    const astu::Vector2d &Item::GetVector2Value() const {
        return state->GetVector2Value();
    }

    std::shared_ptr<Item> Item::GetListElement(size_t idx, unsigned int lineNumber) {
        return state->GetListElement(idx, lineNumber);
    }

    void Item::AppendListElement(std::shared_ptr<Item> elem) {
        return state->AppendListElement(elem);
    }

    bool Item::IsReference() const {
        return state->IsReference();
    }

}