// Local includes
#include "ItemStateColor.h"
#include "ItemStateRealMember.h"
#include "InterpreterFunction.h"
#include "InterpreterError.h"
#include "InterpreterFunctionOneParameter.h"
#include "InterpreterFunctionTwoParameter.h"
#include "InterpreterFunctionNoParameter.h"
#include "Item.h"

// C++ Standard Library includes.
#include <functional>

using namespace std;
using namespace astu;

namespace velox {

    ItemStateColor::ItemStateColor(const Color4d &inValue) : value(inValue) {

        AddItem("Saturate", InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
            this->value.Saturate();
            return Item::CreateUndefined();
        }));

        AddItem("Lerp", InterpreterFunctionTwoParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param1, std::shared_ptr<Item> param2,
                          unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param1->GetType() != ItemType::Color4) {
                        throw InterpreterError("first parameter for method 'Lerp' must be of type color", lineNumber);
                    }

                    if (!param2->IsNumber()) {
                        throw InterpreterError("second parameter for method 'Lerp' must be a number", lineNumber);
                    }

                    return Item::CreateColor(value.Lerp(param1->GetColorValue(), param2->GetRealValue(lineNumber)));
                }));

        AddItem("Blend", InterpreterFunctionOneParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param->GetType() != ItemType::Color4) {
                        throw InterpreterError("parameter for method 'Blend' must be of type color", lineNumber);
                    }

                    value.Blend(param->GetColorValue());
                    return Item::CreateUndefined();
                }));

        AddItem("red", Item::Create(std::make_unique<ItemStateRealMember>(value.r)));
        AddItem("green", Item::Create(std::make_unique<ItemStateRealMember>(value.g)));
        AddItem("blue", Item::Create(std::make_unique<ItemStateRealMember>(value.b)));
        AddItem("alpha", Item::Create(std::make_unique<ItemStateRealMember>(value.a)));
    }

    unique_ptr<ItemState> ItemStateColor::Copy() const {
        return make_unique<ItemStateColor>(value);
    }

    int ItemStateColor::GetIntegerValue(unsigned int lineNumber) const {
        return value.GetARGB();
    }

    std::string ItemStateColor::GetStringValue(ScriptContext &sc) const {
        if (value.a < 1) {
            return "|" + to_string(value.r) + ", " + to_string(value.g) + ", " + to_string(value.b) + ", " +
                   to_string(value.a) + "|";
        }
        return "|" + to_string(value.r) + ", " + to_string(value.g) + ", " + to_string(value.b) + "|";
    }

    const Color4d & ItemStateColor::GetColorValue() const {
        return value;
    }

    ItemType ItemStateColor::GetType() const {
        return ItemType::Color4;
    }

    bool ItemStateColor::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Color4)
            return false;

        value = rhs.GetColorValue();
        return true;
    }

}