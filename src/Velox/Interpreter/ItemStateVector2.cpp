/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/ItemStateVector2.h"
#include "Velox/Interpreter/ItemStateRealMember.h"
#include "Velox/Interpreter/InterpreterFunction.h"
#include "Velox/Interpreter/InterpreterError.h"
#include "Velox/Interpreter/InterpreterFunctionOneParameter.h"
#include "Velox/Interpreter/InterpreterFunctionTwoParameter.h"
#include "Velox/Interpreter/InterpreterFunctionNoParameter.h"
#include "Velox/Interpreter/Item.h"

// C++ Standard Library includes.
#include <functional>

using namespace std;
using namespace astu;

namespace velox {

    ItemStateVector2::ItemStateVector2(const Vector2d &inValue) : value(inValue) {

        AddItem("SetZero",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    this->value.SetZero();
                    return Item::CreateUndefined();
                }));

        AddItem("IsZero",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateBoolean(value.IsZero());
                }));

        AddItem("IsUnitVector",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateBoolean(value.IsUnitVector());
                }));

        AddItem("Flip",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    this->value.Flip();
                    return Item::CreateUndefined();
                }));

        AddItem("Normalize",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    this->value.Normalize();
                    return Item::CreateUndefined();
                }));

        AddItem("Perpendicularize",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    this->value.Perpendicularize();
                    return Item::CreateUndefined();
                }));

        AddItem("Length",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(value.Length());
                }));

        AddItem("LengthSquared",
                InterpreterFunctionNoParameter::CreateItem([this](unsigned int lineNumber) -> std::shared_ptr<Item> {
                    return Item::CreateReal(value.LengthSquared());
                }));

        AddItem("Rotate", InterpreterFunctionOneParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param->IsNumber()) {
                        throw InterpreterError("parameter for method 'Rotate' must be numerical", lineNumber);
                    }

                    value.Rotate(param->GetRealValue(lineNumber));
                    return Item::CreateUndefined();
                }));

        AddItem("RotateDeg", InterpreterFunctionOneParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param->IsNumber()) {
                        throw InterpreterError("parameter for method 'Rotate' must be numerical", lineNumber);
                    }

                    value.RotateDeg(param->GetRealValue(lineNumber));
                    return Item::CreateUndefined();
                }));

        AddItem("Dot", InterpreterFunctionOneParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param->GetType() != ItemType::Vector2) {
                        throw InterpreterError("parameter for method 'Dot' must be Vector2", lineNumber);
                    }

                    return Item::CreateReal(value.Dot(param->GetVector2Value()));
                }));

        AddItem("Cross", InterpreterFunctionOneParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param->GetType() != ItemType::Vector2) {
                        throw InterpreterError("parameter for method 'Cross' must be Vector2", lineNumber);
                    }

                    return Item::CreateReal(value.Cross(param->GetVector2Value()));
                }));

        AddItem("Angle", InterpreterFunctionOneParameter::CreateItem(
                [this](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item>
                {
                    if (param->GetType() != ItemType::Vector2) {
                        throw InterpreterError("parameter for method 'Angle' must be Vector2", lineNumber);
                    }

                    return Item::CreateReal(value.Angle(param->GetVector2Value()));
                }));


        //AddItem("Length", InterpreterFunctionTwoParameter::CreateItem(
        //        [this](ScriptContext &sc, std::shared_ptr<Item> param1, std::shared_ptr<Item> param2,
        //                  unsigned int lineNumber) -> std::shared_ptr<Item>
        //        {
        //            if (param1->GetType() != ItemType::Color4) {
        //                throw InterpreterError("first parameter for method 'Lerp' must be of type color", lineNumber);
        //            }
        //
        //            if (!param2->IsNumber()) {
        //                throw InterpreterError("second parameter for method 'Lerp' must be a number", lineNumber);
        //            }
        //
        //            return Item::CreateColor(value.Lerp(param1->GetColorValue(), param2->GetRealValue(lineNumber)));
        //        }));

        AddItem("x", Item::Create(std::make_unique<ItemStateRealMember>(value.x)));
        AddItem("y", Item::Create(std::make_unique<ItemStateRealMember>(value.y)));
    }

    unique_ptr<ItemState> ItemStateVector2::Copy() const {
        return make_unique<ItemStateVector2>(value);
    }

    std::string ItemStateVector2::GetStringValue(ScriptContext &sc) const {
        return "<" + to_string(value.x) + ", " + to_string(value.y) + ">";
    }

    ItemType ItemStateVector2::GetType() const {
        return ItemType::Vector2;
    }

    bool ItemStateVector2::Assign(Item &owner, const ItemState &rhs) {
        if (rhs.GetType() != ItemType::Vector2)
            return false;

        value = rhs.GetVector2Value();
        return true;
    }

    const Vector2d &ItemStateVector2::GetVector2Value() const {
        return value;
    }

    shared_ptr<Item> ItemStateVector2::ExecuteUnaryMinus() const {
        return Item::CreateVector2(-value);
    }

}