/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Interpreter/Item.h"
#include "Velox/Interpreter/ItemType.h"
#include "Velox/Interpreter/InterpreterError.h"

namespace velox {

    std::shared_ptr<Item>
    Item::ExecuteArithmeticOperator(ScriptContext &sc,
                                    ArithmeticOperator op,
                                    std::shared_ptr<Item> item,
                                    unsigned int lineNumber) const
    {
        switch(GetType()) {

            case ItemType::Undefined:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Integer:
                switch(item->GetType()) {

                    case ItemType::Undefined:
                        break;

                    case ItemType::Integer:
                        switch(op) {
                            case ArithmeticOperator::ADD:
                                return Item::CreateInteger(
                                        state->GetIntegerValue(lineNumber) + item->state->GetIntegerValue(lineNumber));
                            case ArithmeticOperator::SUB:
                                return Item::CreateInteger(
                                        state->GetIntegerValue(lineNumber) - item->state->GetIntegerValue(lineNumber));
                            case ArithmeticOperator::MUL:
                                return Item::CreateInteger(
                                        state->GetIntegerValue(lineNumber) * item->state->GetIntegerValue(lineNumber));
                            case ArithmeticOperator::DIV:
                                return Item::CreateInteger(
                                        state->GetIntegerValue(lineNumber) / item->state->GetIntegerValue(lineNumber));
                            case ArithmeticOperator::MOD:
                                return Item::CreateInteger(
                                        state->GetIntegerValue(lineNumber) % item->state->GetIntegerValue(lineNumber));

                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }

                    case ItemType::Real:
                        switch(op) {
                            case ArithmeticOperator::ADD:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) + item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::SUB:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) - item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::MUL:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) * item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::DIV:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) / item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::MOD:
                                return Item::CreateReal(fmod(state->GetRealValue(lineNumber),
                                                             item->state->GetRealValue(lineNumber)));
                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        switch (op) {
                            case ArithmeticOperator::MUL:
                                return Item::CreateColor(state->GetRealValue(lineNumber) * item->state->GetColorValue());
                            case ArithmeticOperator::DIV:
                                return Item::CreateColor(state->GetRealValue(lineNumber) / item->state->GetColorValue());
                            default:
                                throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                        }

                    case ItemType::Vector2:
                        switch(op) {
                            case ArithmeticOperator::MUL:
                                return Item::CreateVector2(
                                        state->GetRealValue(lineNumber) * item->state->GetVector2Value());
                            case ArithmeticOperator::DIV:
                                return Item::CreateVector2(
                                        state->GetRealValue(lineNumber) / item->state->GetVector2Value());
                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Real:
                switch(item->GetType()) {

                    case ItemType::Undefined:
                        break;

                    case ItemType::Integer:
                        // Fall-through

                    case ItemType::Real:
                        switch(op) {
                            case ArithmeticOperator::ADD:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) + item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::SUB:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) - item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::MUL:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) * item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::DIV:
                                return Item::CreateReal(
                                        state->GetRealValue(lineNumber) / item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::MOD:
                                return Item::CreateReal(fmod(state->GetRealValue(lineNumber),
                                                             item->state->GetRealValue(lineNumber)));
                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        switch (op) {
                            case ArithmeticOperator::MUL:
                                return Item::CreateColor(state->GetRealValue(lineNumber) * item->GetColorValue());

                            default:
                                throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                        }

                    case ItemType::Vector2:
                        switch(op) {
                            case ArithmeticOperator::MUL:
                                return Item::CreateVector2(
                                        state->GetRealValue(lineNumber) * item->state->GetVector2Value());
                            case ArithmeticOperator::DIV:
                                return Item::CreateVector2(
                                        state->GetRealValue(lineNumber) / item->state->GetVector2Value());
                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }

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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Boolean:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Color3:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Color4:
                switch(item->GetType()) {

                    case ItemType::Undefined:
                        break;

                    case ItemType::Integer:
                        // Fall-through.

                    case ItemType::Real:
                        switch (op) {
                            case ArithmeticOperator::MUL:
                                return Item::CreateColor(state->GetColorValue() * item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::DIV:
                                return Item::CreateColor(state->GetColorValue() / item->state->GetRealValue(lineNumber));
                            default:
                                throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                        }
                        break;

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        switch (op) {
                            case ArithmeticOperator::ADD:
                                return Item::CreateColor(state->GetColorValue() + item->state->GetColorValue());
                            case ArithmeticOperator::SUB:
                                return Item::CreateColor(state->GetColorValue() - item->state->GetColorValue());
                            case ArithmeticOperator::MUL:
                                return Item::CreateColor(state->GetColorValue() * item->state->GetColorValue());
                            case ArithmeticOperator::DIV:
                                return Item::CreateColor(state->GetColorValue() / item->state->GetColorValue());
                            default:
                                throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                        }
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Vector2:
                switch(item->GetType()) {

                    case ItemType::Undefined:
                        break;

                    case ItemType::Integer:
                        switch(op) {
                            case ArithmeticOperator::MUL:
                                return Item::CreateVector2(state->GetVector2Value() * item->state->GetIntegerValue(lineNumber));
                            case ArithmeticOperator::DIV:
                                return Item::CreateVector2(state->GetVector2Value() / item->state->GetIntegerValue(lineNumber));
                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }
                        break;

                    case ItemType::Real:
                        switch(op) {
                            case ArithmeticOperator::MUL:
                                return Item::CreateVector2(state->GetVector2Value() * item->state->GetRealValue(lineNumber));
                            case ArithmeticOperator::DIV:
                                return Item::CreateVector2(state->GetVector2Value() / item->state->GetRealValue(lineNumber));
                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }

                    case ItemType::Boolean:
                        break;

                    case ItemType::Color3:
                        break;

                    case ItemType::Color4:
                        break;

                    case ItemType::Vector2:
                        switch(op) {
                            case ArithmeticOperator::ADD:
                                return Item::CreateVector2(state->GetVector2Value() + item->state->GetVector2Value());
                            case ArithmeticOperator::SUB:
                                return Item::CreateVector2(state->GetVector2Value() - item->state->GetVector2Value());
                            case ArithmeticOperator::MUL:
                                return Item::CreateVector2(state->GetVector2Value() * item->state->GetVector2Value());
                            case ArithmeticOperator::DIV:
                                return Item::CreateVector2(state->GetVector2Value() / item->state->GetVector2Value());
                            default:
                                throw std::logic_error("Internal interpreter error: unknown arithmetic operator");
                        }

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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Vector3:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Matrix3:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Matrix4:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::Quaternion:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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
                            case ArithmeticOperator::ADD:
                                return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                        }
                        break;

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            case ItemType::String:
                switch (op) {
                    case ArithmeticOperator::ADD:
                        return Item::CreateString(state->GetStringValue(sc) + item->state->GetStringValue(sc));
                }
                break;

            case ItemType::Other:
                switch(item->GetType()) {

                    case ItemType::Undefined:
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

                    case ItemType::Other:
                        break;

                    default:
                        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);
                }
                break;

            default:
                throw InterpreterError("undefined arithmetic operator between this types", lineNumber);

        }	// end of main-switch

        // Look for custom operation function within this item for overloaded operators.
        auto opFunc = state->FindItem(arithmeticOperatorName[static_cast<int>(op)]);
        if (opFunc) {
            InterpreterItemParameterList params;
            params.AddParameter(item);
            return opFunc->CallAsFunction(sc, params, lineNumber);
        }

        throw InterpreterError("undefined arithmetic operator between this types", lineNumber);

    } // end of method

} // end of namespace
