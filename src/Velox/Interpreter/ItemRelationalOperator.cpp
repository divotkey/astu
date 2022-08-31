// Copyright (c) 2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Velox/Interpreter/Item.h"

namespace velox {

    std::shared_ptr<Item>
    Item::ExecuteRelationalOperator(ScriptContext &sc,
                                    RelationalOperator op,
                                    std::shared_ptr<Item> item,
                                    unsigned int lineNumber) const {
        switch (GetType()) {

            case ItemType::Undefined:
                switch (op) {

                    case RelationalOperator::EQUAL:
                        if (item->IsUndefined())
                            return Item::CreateBoolean(true);
                        else
                            return Item::CreateBoolean(false);

                    case RelationalOperator::NOT_EQUAL:
                        if (item->IsUndefined())
                            return Item::CreateBoolean(false);
                        else
                            return Item::CreateBoolean(true);

                    default:
                        return Item::CreateBoolean(true);
                }

            case ItemType::Integer:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        switch (op) {
                            case RelationalOperator::LESS_THAN:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) < item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::LESS_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) <= item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::GREATER_THAN:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) > item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::GREATER_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) >= item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) == item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) != item->state->GetIntegerValue(lineNumber));
                        }
                        break;

                    case ItemType::Real:
                        switch (op) {
                            case RelationalOperator::LESS_THAN:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) < item->state->GetRealValue(lineNumber));
                            case RelationalOperator::LESS_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) <= item->state->GetRealValue(lineNumber));
                            case RelationalOperator::GREATER_THAN:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) > item->state->GetRealValue(lineNumber));
                            case RelationalOperator::GREATER_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) >= item->state->GetRealValue(lineNumber));
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) == item->state->GetRealValue(lineNumber));
                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetIntegerValue(lineNumber) != item->state->GetRealValue(lineNumber));
                        }
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Real:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        switch (op) {
                            case RelationalOperator::LESS_THAN:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) < item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::LESS_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) <= item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::GREATER_THAN:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) > item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::GREATER_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) >= item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) == item->state->GetIntegerValue(lineNumber));
                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) != item->state->GetIntegerValue(lineNumber));
                        }
                        break;

                    case ItemType::Real:
                        switch (op) {
                            case RelationalOperator::LESS_THAN:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) < item->state->GetRealValue(lineNumber));
                            case RelationalOperator::LESS_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) <= item->state->GetRealValue(lineNumber));
                            case RelationalOperator::GREATER_THAN:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) > item->state->GetRealValue(lineNumber));
                            case RelationalOperator::GREATER_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) >= item->state->GetRealValue(lineNumber));
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) == item->state->GetRealValue(lineNumber));
                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetRealValue(lineNumber) != item->state->GetRealValue(lineNumber));
                        }

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Boolean:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(
                                        state->GetBooleanValue() == item->state->GetBooleanValue());
                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetBooleanValue() != item->state->GetBooleanValue());
                        }
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Color3:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Color4:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(
                                        state->GetColorValue() == item->state->GetColorValue());
                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetColorValue() != item->state->GetColorValue());
                        }

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Vector2:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Vector3:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Matrix3:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Matrix4:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Quaternion:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::String:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        switch (op) {
                            case RelationalOperator::LESS_THAN:
                                return Item::CreateBoolean(
                                        state->GetStringValue(sc) < item->state->GetStringValue(sc));
                            case RelationalOperator::LESS_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetStringValue(sc) <= item->state->GetStringValue(sc));
                            case RelationalOperator::GREATER_THAN:
                                return Item::CreateBoolean(
                                        state->GetStringValue(sc) > item->state->GetStringValue(sc));
                            case RelationalOperator::GREATER_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetStringValue(sc) >= item->state->GetStringValue(sc));
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(
                                        state->GetStringValue(sc) == item->state->GetStringValue(sc));
                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(
                                        state->GetStringValue(sc) != item->state->GetStringValue(sc));

                        }

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            case ItemType::Other:
                switch (item->GetType()) {

                    case ItemType::Undefined:
                        switch (op) {
                            case RelationalOperator::EQUAL:
                                return Item::CreateBoolean(false);

                            case RelationalOperator::NOT_EQUAL:
                                return Item::CreateBoolean(true);
                        }
                        break;

                    case ItemType::Integer:
                        break;

                    case ItemType::Real:
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        break;

                    case ItemType::Vector3:
                        break;

                    case ItemType::Matrix3:
                        break;

                    case ItemType::Matrix4:
                        break;

                    case ItemType::Quaternion:
                        break;

                    case ItemType::String:
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("relational operation between this two types is undefined", lineNumber);
                }
                break;

            default:
                throw InterpreterError("relational operation between this two types is undefined", lineNumber);

        }    // end of main-switch

        // Look for custom operation function within this item for overloaded operators.
        auto opFunc = state->FindItem(relationalOperatorName[static_cast<int>(op)]);
        if (opFunc) {
            InterpreterItemParameterList params;
            params.AddParameter(item);
            return opFunc->CallAsFunction(sc, params, lineNumber);
        }

        throw InterpreterError("relational operation between this two types is undefined", lineNumber);

    } // end of method

} // end of namespace
