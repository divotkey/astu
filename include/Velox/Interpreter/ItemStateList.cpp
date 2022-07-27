// Local includes
#include "ItemStateList.h"
#include "ItemStateRealMember.h"
#include "InterpreterFunction.h"
#include "InterpreterError.h"
#include "InterpreterFunctionNoParameter.h"
#include "InterpreterFunctionOneParameter.h"
#include "Item.h"

// C++ Standard Library includes.
#include <functional>

using namespace std;
using namespace astu;

namespace velox {

    ItemStateList::ItemStateList() {

        AddItem("size", InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
            return Item::CreateInteger(static_cast<int>(elements.size()));
        }));

        AddItem("append", InterpreterFunctionOneParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    AppendListElement(param);
                    return Item::CreateUndefined();
                }));
    }

    unique_ptr<ItemState> ItemStateList::Copy() const {
        auto result = make_unique<ItemStateList>();
        for (auto elem : elements) {
            result->elements.push_back(elem);
        }
        return result;
    }

    std::string ItemStateList::GetStringValue(ScriptContext &sc) const {
        if (elements.empty()) {
            return "[]";
        }

        std::string result("[");
        result += elements[0]->GetStringValue(sc);
        for (size_t i = 1; i < elements.size(); ++i) {
            result += ", " + elements[i]->GetStringValue(sc);
        }
        result += "]";
        return result;
    }

    ItemType ItemStateList::GetType() const {
        return ItemType::List;
    }

    bool ItemStateList::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::List)
            return false;

        elements = static_cast<const ItemStateList&>(rhs).elements;
        return true;
    }

    shared_ptr<Item> ItemStateList::GetListElement(size_t idx, unsigned int lineNumber) {
        if (idx >= elements.size()) {
            throw InterpreterError(
                    "List index out of range, list size is " + to_string(elements.size()) + " but index is " +
                    to_string(idx), lineNumber);
        }

        return elements[idx];
    }

    void ItemStateList::AppendListElement(std::shared_ptr<Item> elem) {
        if (elem->IsReference() || !elem->IsOther()) {
            elements.push_back(elem);
        } else {
            elements.push_back(Item::CreateReference(elem));
        }
    }

}